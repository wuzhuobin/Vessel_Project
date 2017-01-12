#include "RegistrationWizard.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RegistrationWizard rw;
	rw.exec();
	return a.exec();
}
