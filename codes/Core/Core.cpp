#include "Core.h"

Core::Core(QObject * parent)
	:QObject(parent)
{

	connect(&mainWindow, SIGNAL(signalImageImportInitialize()),
		&ioManager, SLOT(cleanListsOfFileNames()));
	connect(&mainWindow, SIGNAL(signalImageImportAdd(QStringList*)),
		&ioManager, SLOT(addToListOfFileNames(QStringList*)));
	connect(&mainWindow, SIGNAL(signalImageImportLoad()),
		&ioManager, SLOT(slotOpenMultiImages()));

	mainWindow.show();
}

Core::~Core()
{
}
