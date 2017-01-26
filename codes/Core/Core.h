#ifndef __CORE_H__
#define __CORE_H__

#include <qobject.h>

#include "MainWindow.h"
#include "ImageManager.h"
#include "IOManager.h"
#include "ImageViewer.h"
#include "InteractorStyleSwitch.h"
#include "InteractorStyles\InteractorStyleWindowLevel.h"



class Core: public QObject
{
	Q_OBJECT
public:

	const static unsigned short NUM_OF_IMAGES = 2;

	Core(QObject* parent = nullptr);
	~Core();

private slots:
	
	void slotIOManagerToImageManager(QList<IOManager::ImageType::Pointer>* images,
		QList<itk::GDCMImageIO::Pointer>* dicoms);
	void slotIOManagerToImageManager();

	void slotNavigation();
	void slotWindowLevel();

	//void slotTest(bool flag);

private:
	MainWindow mainWindow;
	IOManager ioManager;
	ImageManager imageManager;

	ImageViewer* imageViewers[MainWindow::NUM_OF_2D_VIEWERS];
	vtkRenderWindowInteractor* imageInteractor[MainWindow::NUM_OF_2D_VIEWERS];
	InteractorStyleSwitch* imageInteractorStyle[MainWindow::NUM_OF_2D_VIEWERS];
	//InteractorStyleWindowLevel* imageInteractorStyle[MainWindow::NUM_OF_2D_VIEWERS];

};



#endif // !__CORE_H__