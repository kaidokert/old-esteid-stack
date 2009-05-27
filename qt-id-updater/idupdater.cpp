#include "idupdater.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>
#include <QDir>
#include "InstallChecker.h"

idupdater::idupdater(QString baseUrl) : QMainWindow(),m_baseUrl(baseUrl) {
	
	setupUi(this);
	manager = new QNetworkAccessManager(this);
	downloadManager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)),
		this, SLOT(netReplyFinished(QNetworkReply*)));
	connect(downloadManager, SIGNAL(finished(QNetworkReply*)),
		this, SLOT(netDownloadFinished(QNetworkReply*)));
}

idupdater::~idupdater() {
	delete manager;
	}

void idupdater::fail(QString msg) {
	m_updateStatus->setText("fail: " + msg);
	}

void idupdater::netReplyFinished(QNetworkReply* reply) {
	m_updateStatus->setText("request completed");
	QDomDocument doc;
	doc.setContent(reply);
	QDomNodeList nodes = doc.elementsByTagName("product");
	if (nodes.length() != 1 ) fail("expected one product");
	}

void idupdater::netDownloadFinished(QNetworkReply* reply) {
	m_updateStatus->setText("Download finished, starting installation...");
	QFile tmp(QDir::tempPath() + "/download.msi") ;
	if (tmp.open(QFile::ReadWrite)) {
		tmp.write(reply->readAll());
		QString tgt = QDir::toNativeSeparators(tmp.fileName());
		m_updateStatus->setText(tgt);
		InstallChecker::installPackage(tgt.toStdWString());
		}
	}

void idupdater::checkUpdates() {
	QUrl url(m_baseUrl + "products.xml");
	if (url.scheme() != "http") fail("only http download supported");
	manager->get(QNetworkRequest(url));
	}

void idupdater::startInstall() {
	QUrl url(m_baseUrl + "EstEIDUtil-20090225.msi");
	QNetworkReply * reply = downloadManager->get(QNetworkRequest(url));
	connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
		this, SLOT(downloadProgress(qint64,qint64)));
	progressBar_2->setValue(0);
	progressBar_2->setMaximum(100);
	}

void idupdater::downloadProgress(qint64 recvd,qint64 total) {
	float perc = recvd * 1.0f/total;
	progressBar_2->setValue( perc * 100.0f);
	m_updateStatus->setText("Downloading...");
	}

void idupdater::startUninstall() {
	}
