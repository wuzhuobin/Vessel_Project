#include "Core.h"
#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <QStyleFactory>


//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
//#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")


int main(int argc, char *argv[])
{
	//itk::Object::GlobalWarningDisplayOff();
	//vtkObject::GlobalWarningDisplayOff();
	QApplication a(argc, argv);

	qApp->setStyle(QStyleFactory::create("Fusion"));


	Core c;
	return a.exec();
}
