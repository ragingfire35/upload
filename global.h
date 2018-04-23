#pragma once

#include <QList>
#include <QString>
#include <QMessageBox>

//const QString domain = "http:://tian.cow8.cn";
//const QString domain = "http://tlsce.59156.cn";
const QString domain = "http://tlsbz.cn";
//const QString domain = "http://tian.cn";
const QString urlfile = domain + "/Home/Interface/uploadFile";
const QString urlimage = domain + "/Home/Interface/uploadPicture";
//const QString urlfile = domain + "/Home/Index/uploadFile?XDEBUG_SESSION_START=PHPSTORM";
//const QString urlimage = domain + "/Home/Index/uploadPicture?XDEBUG_SESSION_START=PHPSTORM";

//const QString urltest = domain + "/index.php/Home/Index/uploadPicture.html";

const QString urljson = domain + "/Home/Interface/batch_add";
const QString expire = "20180601";

enum 
{
	TYPE_MUSIC,
	TYPE_IMAGE
};

struct FileId {
	QString fileName;
	int id;
	QString remark;
	int sort;
};


static void WriteLog(QString filename,QString str) {	
	QFile file(filename);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append)){
		QMessageBox::warning(NULL, "file write", "can't open", QMessageBox::Yes);		
	}
	QTextStream in(&file);
	in << str << "\n";
}

static bool IsDigitStr(QString src) {
	QByteArray ba = src.toLatin1();//QString 转换为 char*
	const char *s = ba.data();
	while (*s && *s >= '0' && *s <= '9') s++;
	if (*s) { //不是纯数字
		return false;
	}
	else { //纯数字
		return true;
	}
}