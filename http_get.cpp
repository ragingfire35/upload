#include "http_get.h"  
#include <shlwapi.h>
#include<windows.h>
#include <tchar.h>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include "global.h"

Http::Http()
{
	fileRealSize=0;
	sendtotalsize=0;
	totalsize=0;
}

Http::~Http()
{
	
}

void Http::Upload(QObject* parent,QObject* mainParent, int type, QString strurl, QString fulldir, QString rdir){

	mType = type;
	mparent = parent;
	mMainParent = mainParent;
	mfulldir = fulldir;
	mrdir = fulldir;
	
	QFileInfo flinfo(fulldir);
	QString name = flinfo.fileName();
	//QString tmp = fulldir.replace("/","\\");
	QFileInfo fn(fulldir);
	if (!fn.exists()) {
		return;
	}
	fileRealSize = fn.size();
	//file = new QFile(tmp);	
	//file->open(QIODevice::ReadOnly);

	QUrl url(strurl);
	accessManager = new QNetworkAccessManager(this);
	request.setUrl(url);
	/******************设置http的header***********************/
	
	QString bd = QString("----WebKitFormBoundaryorvO6Em3JtSmg2QT");
	QString fm = QString("multipart/form-data; boundary="+bd);

	request.setHeader(QNetworkRequest::ContentTypeHeader, fm.toUtf8());  
	request.setHeader(QNetworkRequest::CookieHeader, "XDEBUG_SESSION=PHPSTORM");

	//request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");	//
	//request.setRawHeader("Content-Disposition",fm.toUtf8()); 
	//request.setHeader(QNetworkRequest::ContentLengthHeader,post_data.length());
	 	 
	//multipart
	QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

	multiPart->setBoundary(bd.toUtf8());

	QHttpPart filePart;
	//filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
	filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));	
	fm = QString("form-data; name=\"file1\";filename=\"%1\"").arg(name);
	filePart.setHeader( QNetworkRequest::ContentDispositionHeader, QVariant(fm.toUtf8()) );

	QFile *file = new QFile(mfulldir);
	file->open(QIODevice::ReadOnly);
	filePart.setBodyDevice(file);
	file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart
	
	multiPart->append(filePart);
	multiPart->setBoundary(bd.toUtf8());

	QHttpPart submit;
	fm = QString("form-data; name=\"submit\"");
	submit.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(fm.toUtf8()));
	QString txt = "111";
	submit.setBody(txt.toLatin1());

	multiPart->append(submit);
	multiPart->setBoundary(bd.toUtf8());


	connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));//finish为manager自带的信号，replyFinished是自定义的 	
	reply = accessManager->post(request, multiPart);//通过发送数据，返回值保存在reply指针里.
	connect(reply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(OnUploadProgress(qint64, qint64)));//download文件时进度  	
}

void GetCodeIdForMusic(QString res, int& code, int& id) {
	QJsonParseError complex_json_error;
	QJsonDocument complex_parse_doucment = QJsonDocument::fromJson(res.toUtf8(), &complex_json_error);

	if (complex_json_error.error == QJsonParseError::NoError) {
		QJsonObject jsonObject = complex_parse_doucment.object();
		if (jsonObject.contains("code")) {
			QJsonValue value = jsonObject.value("code");
			if (value.isDouble()) {
				code = value.toInt();
			}
		}		
		if (jsonObject.contains("data")) {
			QJsonValue value = jsonObject.value("data");
			if (value.isObject()) {
				QJsonObject obj = value.toObject();
				if (obj.contains("id")) {
					QJsonValue value = obj.value("id");
					if (value.isString()) {
						QString strid = value.toString();
						id = strid.toInt();
					}
				}
			}
		}
	}
}
//file
void GetCodeId(QString res,int& code,int& id) {
	QJsonParseError complex_json_error;
	QJsonDocument complex_parse_doucment = QJsonDocument::fromJson(res.toUtf8(), &complex_json_error);
	
	if (complex_json_error.error == QJsonParseError::NoError) {
		QJsonObject jsonObject = complex_parse_doucment.object();
		if (jsonObject.contains("code")) {
			QJsonValue value = jsonObject.value("code");
			if (value.isDouble()) {
				code = value.toInt();
			}
		}
		if (jsonObject.contains("id")) {
			QJsonValue value = jsonObject.value("id");
			if ( value.isDouble() ) {
				id = value.toInt();
				
			}
		}
	}
}

/***************响应结束**************************/
void Http::replyFinished(QNetworkReply *reply) {
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
		QString str = result.toUtf8();
		//qDebug() << result;
		int code = 0;
		int id = 0;
		//if (TYPE_IMAGE == mType) {
			GetCodeId(str, code, id);
		//}
		//if (TYPE_MUSIC == mType) {
			//GetCodeIdForMusic(str, code, id);
		//}
		if (200 != code || 0==id) {
			QMessageBox::information(NULL, "return json error", str+mfulldir, QMessageBox::Yes);
			return;
		}
		//return ok
		QString retVal;
		QMetaObject::invokeMethod(mparent, "HttpSuccessCallBack", Qt::DirectConnection,
			Q_RETURN_ARG(QString, retVal),
			Q_ARG(int,mType),
			Q_ARG(QString,mrdir),
			Q_ARG(QString,result),
			Q_ARG(int,id)
			);
	}
	else
	{
		//处理错误
	}

	reply->deleteLater();//要删除reply，但是不能在repyfinished里直接delete，要调用deletelater;
	this->deleteLater();
}

/***********更新进度条*************/
void Http::OnUploadProgress(qint64 bytesSent, qint64 bytesTotal) {

	sendtotalsize += bytesSent;
	totalsize = bytesTotal;

	QString retVal;
	QMetaObject::invokeMethod(mMainParent, "HttpProgressCallback", Qt::DirectConnection,
		Q_RETURN_ARG(QString, retVal),
		Q_ARG(qint64, bytesSent),
		Q_ARG(qint64, bytesTotal)
	);
}
