#include "Core.h"
#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <QStyleFactory>
#include "EncryptionAuthentication.h"
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
//#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")


int main(int argc, char *argv[])
{
	//itk::Object::GlobalWarningDisplayOff();
	//vtkObject::GlobalWarningDisplayOff();
	QApplication a(argc, argv);

	a.setStyle(QStyleFactory::create("Fusion"));
	EncryptionAuthentication ea(0, QString(), QString(),
		QDateTime(QDate(2017, 06, 10),
			QTime(24, 0, 0)),
		"39431068");

	// disable reminder of the expire date
	ea.enableExpiredDateTimeHint(false);
	// enable reminder of the expire date
	//ea.enableExpiredDateTimeHint(true);
	if (ea.authenticationExecAndKeyType(
		EncryptionAuthentication::HAVING_KEY |
		EncryptionAuthentication::USER_PASSWORD |
		EncryptionAuthentication::EXPIRED_DATE_TIME) != EncryptionAuthentication::NORMAL) {
		return EXIT_FAILURE;
	}
	else {
		Core c;
		return a.exec();
	}

;
	
}
