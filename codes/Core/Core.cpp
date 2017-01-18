#include "Core.h"

Core::Core(QObject * parent)
	:QObject(parent)
{
	mainWindow.show();

	connect(&mainWindow, SIGNAL(signalImageImportInitialize()),
		&ioManager, SLOT(cleanListsOfFileNames()));
	connect(&mainWindow, SIGNAL(signalImageImportAdd(QStringList*)),
		&ioManager, SLOT(addToListOfFileNames(QStringList*)));
	connect(&mainWindow, SIGNAL(signalImageImportLoad()),
		&ioManager, SLOT(slotOpenMultiImages()));


}

Core::~Core()
{
}
