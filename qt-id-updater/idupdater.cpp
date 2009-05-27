#include "idupdater.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>
#include <QDir>
#include <QTime>
#include "InstallChecker.h"

idupdater::idupdater(QString baseUrl,bool autocheck,bool autoupdate) : QMainWindow(),
	m_autoupdate(autoupdate),m_baseUrl(baseUrl) {
	
	setupUi(this);
	manager = new QNetworkAccessManager(this);
	downloadManager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)),
		this, SLOT(netReplyFinished(QNetworkReply*)));
	connect(downloadManager, SIGNAL(finished(QNetworkReply*)),
		this, SLOT(netDownloadFinished(QNetworkReply*)));
	groupBoxProduct->setVisible(false);
	enableInstall(false);
	if (autocheck) 
		m_checkUpdatesButton->click();
}

idupdater::~idupdater() {
	delete manager;
	}

void idupdater::enableInstall(bool enable) {
	m_installButton->setEnabled(enable);
	m_uninstallButton->setEnabled(enable);
	if (enable && m_autoupdate)
		m_installButton->click();
	}

void idupdater::status(QString msg) {
	m_updateStatus->setText(msg);
	}
void idupdater::fail(QString msg) {
	m_updateStatus->setText("fail: " + msg);
	}

void idupdater::netReplyFinished(QNetworkReply* reply) {
	groupBoxProduct->setVisible(true);
	status("Check completed");
	QDomDocument doc;
	doc.setContent(reply);
	QDomNodeList nodes = doc.elementsByTagName("product");
	//if (nodes.length() != 1 ) fail("expected one product");
	product = nodes.item(0).toElement();
	QString code = product.attribute("UpgradeCode");
	std::wstring version,availableVersion;
	InstallChecker::getInstalledVersion(code.toStdWString(),version);
	availableVersion = product.attribute("ProductVersion").toStdWString();

	m_productName->setText(product.attribute("ProductName","none"));
	m_productDescription->setText(product.attribute("ProductName","none"));
	m_installedVer->setText(QString::fromStdWString(version));
	m_availableVer->setText(product.attribute("ProductVersion","0"));
	
	enableInstall( version != availableVersion);
	}

void idupdater::netDownloadFinished(QNetworkReply* reply) {
	status("Download finished, starting installation...");
	QFile tmp(QDir::tempPath() + "/" + product.attribute("filename")) ;
	if (tmp.open(QFile::ReadWrite)) {
		tmp.write(reply->readAll());
		QString tgt = QDir::toNativeSeparators(tmp.fileName());
		if (!InstallChecker::verifyPackage(tgt.toStdWString()))
			fail("Downloaded package integrity check failed");
		InstallChecker::installPackage(tgt.toStdWString());
		status("Package installed");
		}
	}

void idupdater::checkUpdates() {
	enableInstall(false);
	status("Checking for update..");	
	QUrl url(m_baseUrl + "products.xml");
	if (url.scheme() != "http") fail("only http download supported");
	manager->get(QNetworkRequest(url));
	}

void idupdater::startInstall() {
	QUrl url(m_baseUrl + product.attribute("filename"));
	QNetworkReply * reply = downloadManager->get(QNetworkRequest(url));
	connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
		this, SLOT(downloadProgress(qint64,qint64)));
	m_downloadProgress->setValue(0);
	}

void idupdater::downloadProgress(qint64 recvd,qint64 total) {
	static QTime lastupdate = QTime::currentTime();
	static qint64 lastRecvd = 0;
	QTime current = QTime::currentTime();
	if (lastupdate.msecsTo(current) > 500) {
		m_downloadStatus->setText(QString::number( recvd - lastRecvd / lastupdate.msecsTo(current) ));
		lastupdate = current;
		lastRecvd = recvd;
		}
	float perc = recvd * 1.0f/total;
	m_downloadProgress->setValue( perc * 100.0f);
	status("Downloading...");
	}

void idupdater::startUninstall() {
	}
