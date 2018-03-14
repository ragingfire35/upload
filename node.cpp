#include "node.h"
#include "http_get.h"
#include "http_json.h"
#include "global.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

Node::Node(QObject* parent) {
	mParent = parent;
	invokeUpjs = false;
	this->parent_id = 0;
}
Node::~Node() {

}

void Node::Upload(QString& root) {

	mRoodir = root;
	UploadAll();
	//this->UploadImage();
}

void Node::UploadAll() {
	if ( 0 == mMusicList.size() ) {
		UploadImage();
		return;
	}
	UploadFile();
}

void Node::UploadFile() {
	QList<QString>::iterator it = mMusicList.begin();
	//for (;it!= mMusicList.end();it++) {
	QString fulldir = *it;
	Http* http = new Http();
	http->Upload(this, mParent, TYPE_MUSIC, urlfile, fulldir, fulldir);
	mMusicList.pop_front();
	//}
}

void Node::UploadImage() {
	if (0 == mImagList.size()) {
		Uploadjson();
		return;
	}
	QList<QString>::iterator it = mImagList.begin();
	//for (; it != mImagList.end(); it++) {
		QString fulldir = *it;
		Http* http = new Http();
		http->Upload(this, mParent,TYPE_IMAGE, urlimage, fulldir, fulldir);
		mImagList.pop_front();
	//}
}

QString Node::HttpSuccessCallBack(int type, QString rdir,QString result,int id) {
	UploadAll();
	if (TYPE_MUSIC == type) {//music
		mFileIdLst.append(id);
		//int index = mMusicList.indexOf(rdir);
		//if (-1 != index) {
		//	mMusicList.removeOne(rdir);
		//	mFileIdLst.append(id);
		//}
		////DealResult(result,type);
		//if (0 == mMusicList.size() && 0 == mImagList.size() && false == invokeUpjs) {
		//	invokeUpjs = true;
		//	Uploadjson();
		//}		
	}
	else if (TYPE_IMAGE == type) {//image
		mImageIdLst.append(id);
		//int index = mImagList.indexOf(rdir);
		//if (-1 != index) {
		//	mImagList.removeOne(rdir);
		//	mImageIdLst.append(id);
		//}
		//if (0 == mMusicList.size() && 0 == mImagList.size() && false == invokeUpjs) {
		//	invokeUpjs = true;
		//	Uploadjson();
		//}
	}
	return "";
}

QString Node::HttpSuccessCallBackJs(QString result) {
	
	QString retVal;
	QMetaObject::invokeMethod(mParent, "UploadSuccesCallBack", Qt::DirectConnection,
		Q_RETURN_ARG(QString, retVal),
		Q_ARG(QString, "")
		);
	this->deleteLater();
	return "";
}

QString Node::GetUploadjson() {

	QJsonArray file;
	QString fileStr;
	int i = 0;
	QList<int>::iterator fileIt = mFileIdLst.begin();
	for (;fileIt!=mFileIdLst.end(); fileIt++) {
		int id = *fileIt;
		QJsonValue value(id);
		file.push_back(value);
		if (0 == i) {
			fileStr = QString::number(id);
			i++;
		}
		else {
			fileStr = fileStr + "," + QString::number(id);
			i++;
		}
	}

	QJsonArray image;
	QString imageStr;
	int j = 0;
	QList<int>::iterator imageIt = mImageIdLst.begin();
	for (; imageIt != mImageIdLst.end(); imageIt++) {
		int id = *imageIt;
		QJsonValue value(id);
		image.push_back(value);
		
		if (0 == j) {
			imageStr = QString::number(id);
			j++;
		}
		else {
			imageStr = imageStr + "," + QString::number(id);
			j++;
		}
	}
	
	QJsonObject json;
	
	json.insert("pid", this->pid);
	json.insert("title", this->title);
	//json.insert("parent_id", this->parent_id);
	//json.insert("file", QJsonValue(file));
	json.insert("file", fileStr);
	json.insert("sf_status", this->sf_status);
	json.insert("price", this->price);
	json.insert("remark", this->remark);
	//json.insert("icon", 0);
	//json.insert("atlas", QJsonValue(image));
	json.insert("atlas", imageStr);
	
	QJsonDocument document;
	document.setObject(json);
	QByteArray byte_array = document.toJson(QJsonDocument::Compact);
	QString json_str(byte_array);

	return json_str;
}

QString Node::Uploadjson(){

	Httpjson* hj = new Httpjson();
	QString tmp = GetUploadjson();
	hj->Upload(this, mParent,urljson, tmp);

	return "";
}