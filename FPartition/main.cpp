#include "FPartition.h"
#include "FPchart.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FPartition w;
	w.show();
	/*FPchart f;
	f.show();*/
	return a.exec();
}
