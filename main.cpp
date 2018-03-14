#include "upload.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	upload w;
	w.show();
	return a.exec();
}
