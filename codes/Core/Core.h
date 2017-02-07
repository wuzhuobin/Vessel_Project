#ifndef __CORE_H__
#define __CORE_H__

#include <qobject.h>


#include "MainWindow.h"
#include "ModuleWidget.h"
#include "ImageManager.h"
#include "IADEIOManager.h"
#include "IADEImageManager.h"
#include "IADEInteractorStyleSwitch.h"
#include "SurfaceViewer.h"
#include <vtkInteractorStyleSwitch.h>


class Core: public QObject
{
	Q_OBJECT
public:

	const static unsigned short NUM_OF_IMAGES = 2;
	const static unsigned short DEFAULT_IMAGE = 0;
	const static enum VIEW_MODE
	{
		MULTIPLANAR_VIEW = 0,
		ALL_AXIAL_VIEW = 1
	};

	Core(QObject* parent = nullptr);
	~Core();

private slots:
	
	void slotIOManagerToImageManager(QList<IOManager::ImageType::Pointer>* images,
		QList<itk::GDCMImageIO::Pointer>* dicoms);
	void slotIOManagerToImageManager();
	void slotOverlayToImageManager();

	void slotNavigation();
	void slotWindowLevel();
	void slotPaintBrush();
	void slotSeedsPlacer();

	void slotMultiPlanarView();
	void slotAllAxialView();
	void slotChangeView(unsigned int viewMode);

	void slotUpdateSurfaceView();

	//void slotTest(bool flag);

private:
	MainWindow mainWindow;
	ModuleWidget moduleWiget;
	IADEIOManager ioManager;
	IADEImageManager imageManager;

	ImageViewer* imageViewers[MainWindow::NUM_OF_2D_VIEWERS];
	vtkRenderWindowInteractor* imageInteractor[MainWindow::NUM_OF_2D_VIEWERS];
	IADEInteractorStyleSwitch* imageInteractorStyle[MainWindow::NUM_OF_2D_VIEWERS];

	SurfaceViewer* surfaceViewer;
	vtkRenderWindowInteractor* surfaceInteractor;
	vtkInteractorStyleSwitch* surfaceInteractorStyle;


	unsigned int m_viewMode = -1;
};



#endif // !__CORE_H__