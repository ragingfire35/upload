#ifndef HTTP_JSON_H  
#define HTTP_JSON_H  
#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class Httpjson : public QObject
{
	Q_OBJECT
public:
	Httpjson();
	~Httpjson();
public slots:
	void Upload(QObject* parent, QObject* mainParent, QString strurl, QString js);
	void replyFinished(QNetworkReply*reply);
	void OnUploadProgress(qint64 bytesSent, qint64 bytesTotal);
private:
	QObject * mMainParent;
	QString mdir;
	QObject * mparent;
	QFileInfo fileInfo;
	QNetworkAccessManager *accessManager;
	QNetworkRequest request;
	QNetworkReply *reply;
};



#endif