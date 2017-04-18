#ifndef __CORE_H__
#define __CORE_H__

#include <qobject.h>


#include "MainWindow.h"
#include "IADEIOManager.h"
#include "IADEImageManager.h"
#include "IADEInteractorStyleSwitch.h"
#include "IADEInteractorStyleSwitch3D.h"
#include "CenterlineSurfaceViewer.h"
#include "ImageViewer.h"
#include "DataProcessor.h"


class Core: public QObject
{
	Q_OBJECT
public:

	const static unsigned short NUM_OF_IMAGES = 4;
	const static unsigned short DEFAULT_IMAGE = 0;
	//const enum VIEW_MODE
	//{
	//	MULTIPLANAR_VIEW = 0,
	//	ALL_AXIAL_VIEW = 1
	//};

	Core(QObject* parent = nullptr);
	~Core();

private slots:
	

	void slotIOManagerToImageManager();
	void slotOverlayToImageManager();
	void slotUpdateMeasurements();
	// Image interactorstyle
	void slotNavigation();
	void slotWindowLevel();
	void slotThreshold();
	void slotWindowlevelThreshold();
	void slotPaintBrush();
	void slotTesting();
	void slotSeedsPlacer();
	void slotVOI();
	void slotTubularVOI();
	void slotRuler();
	void slotMaximumWallThickness();
	void slotPolygonDraw();
	void slotPolygonDrawSeries();
	void slotVesselSegmentation();

	// Surface interactorStyle
	void slotTrackballCamera();
	void slotCenterLine();
	void slotFindMaximumRadius();
	void slotPerpendicularMeasurement();
	void slotCurvedNavigation();
	void slotWaypoint();
	void slotStenosis();

	// IADE specified
	void slotVBDSmoker();

	// Curved image update
	void slotInitializeCurvedImage();

	void slotChangeImage(QAction* action);
	void slotChangeImage(int viewer, int image);
	void slotChangeCurved(int viewer, bool curvedFlag);
	void slotChangeSliceOrientation(int viewer, int sliceOrientation);
	void slotChangeSliceOrientationToYZ(int viewer);
	void slotChangeSliceOrientationToXZ(int viewer);
	void slotChangeSliceOrientationToXY(int viewer);
	void slotUpdateImageViewersToCurrent(int viewer);
	//void slotMultiPlanarView();
	//void slotAllAxialView();
	void slotCurvedView(bool flag);
	//void slotChangeView(unsigned int viewMode);

	void slotUpdateSurfaceView();

	// label opacity
	//void slotChangeOpacity(int opacity);
	//void slotUpdateOpacity(int opacity);
	void slotRenderALlViewers();


private:
	MainWindow mainWindow;
	IADEIOManager ioManager;
	IADEImageManager imageManager;
	DataProcessor dataProcessor;


	int currentImage[MainWindow::NUM_OF_2D_VIEWERS] = {DEFAULT_IMAGE};
	bool currentCurved[MainWindow::NUM_OF_2D_VIEWERS] = { false };
	int currentSliceOrientation[MainWindow::NUM_OF_2D_VIEWERS] = {
		ImageViewer::SLICE_ORIENTATION_YZ,
		ImageViewer::SLICE_ORIENTATION_XZ,
		ImageViewer::SLICE_ORIENTATION_XY};
	//int currentMode[MainWindow::NUM_OF_2D_VIEWERS] = { MULTIPLANAR_VIEW };
	ImageViewer* imageViewers[MainWindow::NUM_OF_2D_VIEWERS];
	IADEInteractorStyleSwitch* imageInteractorStyle[MainWindow::NUM_OF_2D_VIEWERS];

	CenterlineSurfaceViewer* surfaceViewer;
	IADEInteractorStyleSwitch3D* surfaceInteractorStyle;


	unsigned int m_viewMode = -1;
};



#endif // !__CORE_H__