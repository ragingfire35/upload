#include "upload.h"
#include<iostream>
#include <QMessageBox> 
#include <QTextStream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QDir>
#include <QTextCodec>
#include "global.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif



upload::upload(QWidget *parent)
	: QMainWindow(parent)
{
	need_download_size = 0;
	now_down_size = 0;
	ui.setupUi(this);
	Init();
}


QString upload::HttpSuccessCallBack(QString dir) {
	return  "";
}


void upload::checkDate() {
	QDate ex = QDate::fromString(expire, "yyyyMMdd");
	QDate nw = QDate::currentDate();
	if (nw > ex) {
		QMessageBox::information(NULL, "请支付三期款", "请支付三期款,谢谢", QMessageBox::Yes | QMessageBox::No);
		exit(0);
	}
}

void upload::InitProgress(qint64 maxprogress){	
	ui.upload_progressBar->setRange(0, maxprogress);
	ui.upload_progressBar->setValue(0);
}
void upload::OnUploadButtonClick() {
	mRoodir = ui.selectdir_lineEdit->text();

	QDir dir(mRoodir);
	if (!dir.exists()) {
		QMessageBox::information(NULL, "根路径不存在", "根路径不存在", QMessageBox::Yes);
		exit(0);
	}
	QString idstr  = ui.onepid_lineEdit->text();
	idstr = idstr.trimmed();
	if (!idstr.size()) {
		QMessageBox::information(NULL, "请输入pid", "请输入pid", QMessageBox::Yes);
		exit(0);		
	}
	onepid = idstr.toInt();	
	RecuseDir(mRoodir, NULL, "");	
	InitProgress(need_download_size + 1 * 1024 * 1024);
	Upload();
}
void upload::InitCom() {
	//for test.
	ui.selectdir_lineEdit->setText("D:\\work\\techAngel\\upan\\哈农练指法");
	ui.onepid_lineEdit->setText("263");

	connect(ui.upload_pushButton, SIGNAL(clicked()), this, SLOT( OnUploadButtonClick()) );
}

void upload::RecuseDir(QString path, Node* p,QString dirName) {
	QDir dir(path);
	dir.setFilter(QDir::AllEntries | QDir::Hidden);
	//QList<QString> list;
	QMap<int, QString> map;
	QList<QString> dirlist;
	foreach(QFileInfo mfi, dir.entryInfoList())
	{
		if (mfi.isFile()) {
			QString kr = mfi.absoluteFilePath();
			QFileInfo fileinfo;
			fileinfo = QFileInfo(kr);
			QString ex = fileinfo.suffix();
			QString name = fileinfo.baseName();
			if (p) {
				if (-1!=ex.indexOf("png") || -1 != ex.indexOf("jpg")) {
					p->mImagList.append(kr);
				}
				if (-1 != ex.indexOf("tls") || -1 != ex.indexOf("tgz") || -1!=ex.indexOf("mp3") || -1 != ex.indexOf("mp4")) {
					p->mMusicList.append(kr);
				}				
				need_download_size += fileinfo.size();
			}			
		}
		else {
			if (mfi.fileName() == "." || mfi.fileName() == "..") {
				continue;
			}
			QString kr = mfi.absoluteFilePath();
			QDir curdir(kr);
			QString name = curdir.dirName();
			Node* ptmp = new Node(this);
			ptmp->pid = onepid;
			ptmp->title = name;
			ptmp->price = 1.1;
			ptmp->remark = "哈";
			ptmp->sf_status = 1;
			mNodeList.append(ptmp);
			RecuseDir(mfi.absoluteFilePath(), ptmp, name);
		}
	}//end forreach
}
void upload::Init() {	
	InitCom();
	checkDate();	
	//PasreConf();
	InitProgress(100);
}
QString upload::HttpProgressCallback(qint64 bytesSent, qint64 bytesTotal) {
	now_down_size += bytesSent;
	ui.upload_progressBar->setValue(now_down_size);
	if (now_down_size > need_download_size && 0 == mNodeList.size()) {
		ui.upload_progressBar->setValue(maxprogress);
	}	
	return "";
}

QString upload::UploadSuccesCallBack(QString param) {
	Upload();
	return "";
}
void upload::Upload() {
	if (0 == mNodeList.size() ) {
		QMessageBox::information(NULL,"结果","全部上传成功。", QMessageBox::Yes | QMessageBox::No);
		return;
	}
	Node* p = mNodeList.front();
	mNodeList.pop_front();
	p->Upload(mRoodir);
}
void upload::PasreConf() {

	QString conf = ReadConf();		
	
	QJsonParseError complex_json_error;
	QJsonDocument complex_parse_doucment = QJsonDocument::fromJson(conf.toUtf8(), &complex_json_error);
	int i = 0;
	if (complex_json_error.error == QJsonParseError::NoError){
		QJsonObject jsonObject = complex_parse_doucment.object();
		if (jsonObject.contains("rootdir")){
			QJsonValue value = jsonObject.value("rootdir");
			if (value.isString()){
				mRoodir = value.toString();
			}
		}
		if (jsonObject.contains("upload")){
			QJsonValue arrays_value = jsonObject.take("upload");
			if (arrays_value.isArray()){
				QJsonArray heeloArray = arrays_value.toArray();

				for (int i = 0; i < heeloArray.size(); i++) {
					QJsonValue value = heeloArray.at(i);
					if (value.isObject()) {
						QJsonObject myobject = value.toObject();
						Node* p = new Node(this);
						if (myobject.contains("fullName")){
							QJsonValue value = myobject.value("fullName");
							if (value.isArray() ){
								QJsonArray ar = value.toArray();
								for (int j = 0; j < ar.size();j++){
									QJsonValue value = ar.at(j);
									if (value.isString()) {
										QString tmp = value.toString();
										//exist,and size.
										QString fulldir;
										if ('/' == mRoodir.at(mRoodir.length() - 1)) {
											fulldir = mRoodir + tmp;
										}
										else {
											fulldir = mRoodir + "/" + tmp;
										}
										QFileInfo flinfo(fulldir);										
										QString tmpdir = fulldir.replace("/", "\\");
										QFileInfo fn(tmpdir);
										if (fn.exists()) {
											p->mMusicList.append(tmp);
											need_download_size += fn.size();
										}
										else {
											QMessageBox::information(NULL, "file not exist", tmpdir, QMessageBox::Yes);
										}
										//end exist.
									}
								}
								
							}
						}
						if (myobject.contains("image")){
							QJsonValue value = myobject.value("image");
							if (value.isArray()){
								QJsonArray ar = value.toArray();
								for (int k=0;k<ar.size();k++){
									QJsonValue value = ar.at(k);
									if (value.isString()){																				
										QString tmp = value.toString();
										//exist,and size.
										QString fulldir;
										if ('/' == mRoodir.at(mRoodir.length() - 1)) {
											fulldir = mRoodir + tmp;
										}
										else {
											fulldir = mRoodir + "/" + tmp;
										}
										QFileInfo flinfo(fulldir);
										QString tmpdir = fulldir.replace("/", "\\");
										QFileInfo fn(tmpdir);
										if (fn.exists()) {
											p->mImagList.append(tmp);
											need_download_size += fn.size();
										}
										else {
											QMessageBox::information(NULL, "file not exist", tmpdir, QMessageBox::Yes);
										}
										//end exist.
									}
								}
							}

						}
						if (myobject.contains("pid")) {
							QJsonValue value = myobject.value("pid");
							if (value.isDouble()) {
								p->pid =  value.toInt();
							}
							
						}
						if (myobject.contains("parent_id")) {
							QJsonValue value = myobject.value("parent_id");
							if (value.isDouble()) {
								p->parent_id = value.toInt();
							}
						}
						if (myobject.contains("sf_status")) {
							QJsonValue value = myobject.value("sf_status");
							if (value.isDouble()) {
								p->sf_status = value.toInt();
							}
						}
						if (myobject.contains("price")) {
							QJsonValue value = myobject.value("price");
							if (value.isDouble()) {
								p->price = value.toDouble();
							}
						}
						if (myobject.contains("remark")) {
							QJsonValue value = myobject.value("remark");
							if (value.isString()) {
								p->remark = value.toString();
							}
						}
						//add to list;
						mNodeList.append(p);
					}
				}
			}
		}
	}
	else{
		QMessageBox::information(NULL, "配置文件错误", "配置文件错误！请检查配置文件！", QMessageBox::Yes | QMessageBox::No);
		exit(0);
	}
	//


}

QString upload::ReadConf()
{
	QFile f("upload.conf");
	QTextCodec* codec = QTextCodec::codecForName("utf8");
	
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::information(NULL, "打开配置文件", "打开配置文件d.conf失败", QMessageBox::Yes | QMessageBox::No);
		exit(0);
	}
	//QTextStream txtInput(&f);
	QString lineStr;
	//while (!txtInput.atEnd())
	while (!f.atEnd())
	{
		//QString temp = txtInput.readLine();
		//lineStr += temp;
		QByteArray qa = f.readLine();
		QString temp = codec->toUnicode(qa);
		lineStr += temp;
	}
	f.close();
	QByteArray qa  = lineStr.toUtf8();
	QString tmp = qa;
	return tmp;
}