#pragma once

#include <QList>
#include <QString>

//const QString domain = "http:://tian.cow8.cn";
const QString domain = "http://tlsce.59156.cn";
//const QString domain = "http://tlsbz.cn";
//const QString domain = "http://tian.cn";
const QString urlfile = domain + "/Home/Interface/uploadFile";
const QString urlimage = domain + "/Home/Interface/uploadPicture";
//const QString urlfile = domain + "/Home/Index/uploadFile?XDEBUG_SESSION_START=PHPSTORM";
//const QString urlimage = domain + "/Home/Index/uploadPicture?XDEBUG_SESSION_START=PHPSTORM";

//const QString urltest = domain + "/index.php/Home/Index/uploadPicture.html";

const QString urljson = domain + "/Home/Interface/batch_two_add";
const QString expire = "20180401";

enum 
{
	TYPE_MUSIC,
	TYPE_IMAGE
};