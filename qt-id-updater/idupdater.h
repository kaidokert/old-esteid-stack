
#include "ui_idupdater.h"

#include <QtNetwork/QNetworkAccessManager>

class idupdater : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT

public:
	idupdater(QString baseUrl);
	~idupdater();

public slots:

private slots:
	void startInstall();
	void startUninstall();
	void checkUpdates();
	void netReplyFinished(QNetworkReply*);
	void netDownloadFinished(QNetworkReply*);
	void downloadProgress(qint64 recvd,qint64 total);

private:
	QString m_baseUrl;
	void fail(QString);
	QNetworkAccessManager *manager;
	QNetworkAccessManager *downloadManager;
};
