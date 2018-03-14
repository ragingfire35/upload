#include "http_json.h"  
#include <shlwapi.h>
#include<windows.h>
#include <tchar.h>

Httpjson::Httpjson()
{
}

Httpjson::~Httpjson()
{

}



void Httpjson::Upload(QObject* parent, QObject* mainParent, QString strurl, QString js) {

	mparent = parent;	

	QUrl url(strurl);
	accessManager = new QNetworkAccessManager(this);
	request.setUrl(url);
	/******************设置http的header***********************/
	// request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");  
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");  
	// request.setRawHeader("Content-Disposition","form-data;name='doc';filename='a.txt'");  
	//request.setHeader(QNetworkRequest::ContentLengthHeader,post_data.length());
	  
	connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));//finish为manager自带的信号，replyFinished是自定义的  
	
	reply = accessManager->post(request, js.toUtf8());//通过发送数据，返回值保存在reply指针里.

	connect(reply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(OnUploadProgress(qint64, qint64)));//download文件时进度  
		
}
/***************响应结束**************************/
void Httpjson::replyFinished(QNetworkReply *reply) {
	//获取响应的信息，状态码为200表示正常  
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	//QByteArray bytes = reply->readAll();  //获取字节
	//QString result(bytes);  //转化为字符串
	//qDebug() << result;

	//无错误返回  
	if (reply->error() == QNetworkReply::NoError)
	{	
		QByteArray bytes = reply->readAll();  //获取字节
		QString result(bytes);  //转化为字符串
		//return ok
		QString retVal;
		QMetaObject::invokeMethod(mparent, "HttpSuccessCallBackJs", Qt::DirectConnection,
			Q_RETURN_ARG(QString, retVal),
			Q_ARG(QString, result));
	}
	else
	{
		//处理错误
	}

	reply->deleteLater();//要删除reply，但是不能在repyfinished里直接delete，要调用deletelater;
	this->deleteLater();
}

/***********更新进度条*************/
void Httpjson::OnUploadProgress(qint64 bytesSent, qint64 bytesTotal) {
	//QString retVal;
	//QMetaObject::invokeMethod(mparent, "HttpProgressCallback", Qt::DirectConnection,
	//	Q_RETURN_ARG(QString, retVal),
	//	Q_ARG(qint64, bytesSent),
	//	Q_ARG(qint64, bytesTotal)
	//);
}
