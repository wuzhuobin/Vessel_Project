#include "Core.h"

Core::Core(QObject * parent)
	:
	imageManager(2, parent),
	ioManager(parent),
	QObject(parent)
{

	connect(&mainWindow, SIGNAL(signalImageImportInitialize()),
		&ioManager, SLOT(cleanListsOfFileNames()));
	connect(&mainWindow, SIGNAL(signalImageImportAdd(QStringList*)),
		&ioManager, SLOT(addToListOfFileNames(QStringList*)));
	connect(&mainWindow, SIGNAL(signalImageImportLoad()),
		&ioManager, SLOT(slotOpenMultiImages()));

	connect(&mainWindow, SIGNAL(signalImageImportLoad(QList<QStringList>*)),
		&ioManager, SLOT(addToListOfFileNamesAndOpen(QList<QStringList>*)));


	mainWindow.show();
}

Core::~Core()
{
}
