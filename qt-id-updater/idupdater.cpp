#include "idupdater.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>

idupdater::idupdater(QString baseUrl) : QMainWindow(),m_baseUrl(baseUrl) {
	
	setupUi(this);
	manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)),
		this, SLOT(netReplyFinished(QNetworkReply*)));
}

idupdater::~idupdater() {
	delete manager;
	}

void idupdater::netReplyFinished(QNetworkReply* reply) {
	m_updateStatus->setText("request completed");
	QDomDocument doc;
	doc.setContent(reply);
	QDomNodeList nodes = doc.elementsByTagName("product");
	if (nodes.length() != 1 ) fail("expected one product");
	}

void idupdater::fail(QString msg) {
	m_updateStatus->setText("fail: " + msg);
	}

void idupdater::startInstall() {
	}

void idupdater::startUninstall() {
	}

void idupdater::checkUpdates() {
	QUrl url(m_baseUrl + "products.xml");
	if (url.scheme() != "http") fail("only http download supported");
	manager->get(QNetworkRequest(url));
	}
