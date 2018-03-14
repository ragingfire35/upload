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
	/******************����http��header***********************/
	// request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");  
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");  
	// request.setRawHeader("Content-Disposition","form-data;name='doc';filename='a.txt'");  
	//request.setHeader(QNetworkRequest::ContentLengthHeader,post_data.length());
	  
	connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));//finishΪmanager�Դ����źţ�replyFinished���Զ����  
	
	reply = accessManager->post(request, js.toUtf8());//ͨ���������ݣ�����ֵ������replyָ����.

	connect(reply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(OnUploadProgress(qint64, qint64)));//download�ļ�ʱ����  
		
}
/***************��Ӧ����**************************/
void Httpjson::replyFinished(QNetworkReply *reply) {
	//��ȡ��Ӧ����Ϣ��״̬��Ϊ200��ʾ����  
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	//QByteArray bytes = reply->readAll();  //��ȡ�ֽ�
	//QString result(bytes);  //ת��Ϊ�ַ���
	//qDebug() << result;

	//�޴��󷵻�  
	if (reply->error() == QNetworkReply::NoError)
	{	
		QByteArray bytes = reply->readAll();  //��ȡ�ֽ�
		QString result(bytes);  //ת��Ϊ�ַ���
		//return ok
		QString retVal;
		QMetaObject::invokeMethod(mparent, "HttpSuccessCallBackJs", Qt::DirectConnection,
			Q_RETURN_ARG(QString, retVal),
			Q_ARG(QString, result));
	}
	else
	{
		//�������
	}

	reply->deleteLater();//Ҫɾ��reply�����ǲ�����repyfinished��ֱ��delete��Ҫ����deletelater;
	this->deleteLater();
}

/***********���½�����*************/
void Httpjson::OnUploadProgress(qint64 bytesSent, qint64 bytesTotal) {
	//QString retVal;
	//QMetaObject::invokeMethod(mparent, "HttpProgressCallback", Qt::DirectConnection,
	//	Q_RETURN_ARG(QString, retVal),
	//	Q_ARG(qint64, bytesSent),
	//	Q_ARG(qint64, bytesTotal)
	//);
}
