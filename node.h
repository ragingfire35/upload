#pragma once
#include <QObject>
#include <QList>

class Node :public QObject 
{
	Q_OBJECT
public:
	Node(QObject* parent);
	~Node();
public:
	void Upload(QString& root);
private:
	void UploadAll();
	void UploadFile();
	void UploadImage();
	QString Uploadjson();
	//void DealResult(QString result,int type);
	QString GetUploadjson();
public slots:
	QString HttpSuccessCallBack(int type, QString rdir, QString result, int id, QString name);
	QString HttpSuccessCallBackJs( QString result);
private:
	QString mRoodir;
public:
	QString title;
	QObject* mParent;
	QList<struct FileId> mFileIdLst;
	QList<int> mImageIdLst;
	bool invokeUpjs;	
	QList<QString> mMusicList;
	QList<QString> mImagList;
	int pid;
	int parent_id;
	int sf_status;
	double price;
	QString remark;
};