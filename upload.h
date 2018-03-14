#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_upload.h"
#include "node.h"

class upload : public QMainWindow
{
	Q_OBJECT

public:
	upload(QWidget *parent = Q_NULLPTR);
	QString ReadConf();
	void PasreConf();
	void checkDate();
	void Init();
	void InitProgress(qint64 maxprogress);
	void InitCom();
	void RecuseDir(QString path, Node* p,QString dirName);
private:
	void Upload();
public slots:
	void OnUploadButtonClick();
	QString HttpSuccessCallBack(QString dir);
	QString UploadSuccesCallBack(QString param);
	QString HttpProgressCallback(qint64 bytesSent,qint64 bytesTotal);
private:
	Ui::uploadClass ui;
private:
	qint64 onepid;
	QString mRoodir;
	QList<Node*> mNodeList;
	qint64 need_download_size;
	qint64 now_down_size;
	qint64 maxprogress;
};
