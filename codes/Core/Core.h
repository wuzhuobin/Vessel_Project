#ifndef __CORE_H__
#define __CORE_H__

#include <qobject.h>

#include "MainWindow.h"
#include "IOManager.h"

class Core: public QObject
{
	Q_OBJECT
public:

	Core(QObject* parent = nullptr);
	~Core();

private:
	MainWindow mainWindow;
	IOManager ioManager;

};



#endif // !__CORE_H__