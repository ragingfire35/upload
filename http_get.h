#ifndef HTTP_DOWNLOAD_H  
#define HTTP_DOWNLOAD_H  
#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class Http : public QObject
{
	Q_OBJECT
public:
	Http();
	~Http();
public slots:
	void Upload(QObject* parent, QObject* mainParent, int type,QString strurl, QString fulldir, QString rdir);
	void replyFinished(QNetworkReply*reply);
	void OnUploadProgress(qint64 bytesSent, qint64 bytesTotal);
private:
	qint64 fileRealSize;
	qint64 sendtotalsize;
	qint64 totalsize;
	int mType;
	QString mfulldir;
	QString mrdir;
	QObject * mparent;
	QObject* mMainParent;
	QFile *file;
	QFileInfo fileInfo;
	QNetworkAccessManager *accessManager;
	QNetworkRequest request;
	QNetworkReply *reply;
};



#endif