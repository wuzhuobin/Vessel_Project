#include "Core.h"

#include "IADEOverlay.h"
#include "ui_MainWindow.h"
#include "ui_ModuleWidget.h"
#include "ui_ViewerWidget.h"
#include "ViewerWidget.h"
#include "ModuleWidget.h"
#include "ui_QAbstractNavigation.h"
#include "MeasurementWidget.h"
#include "LabelWidget.h"


#include <qdebug.h>
#include <QVTKInteractor.h>
#include <qsignalmapper.h>

#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkLookupTable.h>

Core::Core(QObject * parent)
	:
	imageManager(NUM_OF_IMAGES, parent),
	ioManager(parent),
	dataProcessor(parent),
	QObject(parent)
{
	ioManager.enableRegistration(true);

	imageManager.setModalityName(0, "Image 0");
	imageManager.setModalityName(1, "Image 1");
	imageManager.setModalityName(2, "Image 2");
	imageManager.setModalityName(3, "Image 3");
	mainWindow.addModalityNames("Image 0");
	mainWindow.addModalityNames("Image 1");
	mainWindow.addModalityNames("Image 2");
	mainWindow.addModalityNames("Image 3");

	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		
		imageViewers[i] = ImageViewer::New();
		imageViewers[i]->SetRenderWindow(mainWindow.getViewerWidget(i)->getUi()->qvtkWidget2->GetRenderWindow());
		imageViewers[i]->SetupInteractor(mainWindow.getViewerWidget(i)->getUi()->qvtkWidget2->GetInteractor());
		//imageViewers[i]->EnableDepthPeelingOn();

		// Never use below method to set the interactorsyle
		//imageInteractorStyle[i]->SetInteractor(imageInteractor[i]);
		imageInteractorStyle[i] = IADEInteractorStyleSwitch::New();
		imageInteractorStyle[i]->SetImageViewer(imageViewers[i]);
		mainWindow.getViewerWidget(i)->getUi()->qvtkWidget2->GetInteractor()->SetInteractorStyle(imageInteractorStyle[i]);
		
	}

	//surfaceInteractor = vtkRenderWindowInteractor::New();

	surfaceViewer = CenterlineSurfaceViewer::New();
	surfaceViewer->SetRenderWindow(mainWindow.getViewerWidget(MainWindow::NUM_OF_VIEWERS - MainWindow::NUM_OF_3D_VIEWERS)->getUi()->qvtkWidget2->GetRenderWindow());
	surfaceViewer->SetupInteractor(mainWindow.getViewerWidget(MainWindow::NUM_OF_VIEWERS - MainWindow::NUM_OF_3D_VIEWERS)->getUi()->qvtkWidget2->GetInteractor());
	surfaceViewer->EnableDepthPeelingOn();

	//surfaceViewer->EnableDepthSortingOn();


	// Never use below method to set the interactorsyle
	//surfaceInteractorStyle[i]->SetInteractor(imageInteractor[i]);
	surfaceInteractorStyle = IADEInteractorStyleSwitch3D::New();
	surfaceInteractorStyle->SetSurfaceViewer(surfaceViewer);
	mainWindow.getViewerWidget(MainWindow::NUM_OF_VIEWERS - MainWindow::NUM_OF_3D_VIEWERS)->getUi()->qvtkWidget2->GetInteractor()->SetInteractorStyle(surfaceInteractorStyle);

	dataProcessor.imageInteractorStyle = imageInteractorStyle;
	dataProcessor.surfaceInteractorStyle = surfaceInteractorStyle;
	dataProcessor.imageManager = &imageManager;

	mainWindow.getUi()->sliceScrollArea->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetNavigation());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetWindowLevel());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetWindowLevelThreshold());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetThreshold());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetPaintBrush());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetLumenSeedsPlacer());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetVOI());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetVBDSmoker());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetTubularVOI());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetRuler());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetMaximumWallThickness());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetPolygonDrawSeries());
	mainWindow.getModuleWidget()->addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetVesselSegmentation2());
	//imageInteractorStyle[DEFAULT_IMAGE]->GetWindowLevel()->show();
	//moduleWiget.setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetWindowLevel());
	

	// connect changing mode
	connect(mainWindow.getUi()->actionTesting, SIGNAL(triggered()),
		this, SLOT(slotTesting()));
	connect(mainWindow.getUi()->actionNavigation, SIGNAL(triggered()),
		this, SLOT(slotNavigation()));
	connect(mainWindow.getUi()->actionWindow_level, SIGNAL(triggered()),
		this, SLOT(slotWindowLevel()));
	connect(mainWindow.getUi()->actionThreshold, SIGNAL(triggered()),
		this, SLOT(slotThreshold()));
	connect(mainWindow.getUi()->actionWindow_level_threshold, SIGNAL(triggered()),
		this, SLOT(slotWindowlevelThreshold()));
	connect(mainWindow.getUi()->actionPaint_brush, SIGNAL(triggered()),
		this, SLOT(slotPaintBrush()));
	connect(mainWindow.getUi()->actionSeeds_placer, SIGNAL(triggered()),
		this, SLOT(slotSeedsPlacer()));
	connect(mainWindow.getUi()->acitonVOI_selection, SIGNAL(triggered()),
		this, SLOT(slotVOI()));
	connect(mainWindow.getUi()->actionTubular_VOI, SIGNAL(triggered()),
		this, SLOT(slotTubularVOI()));
	connect(mainWindow.getUi()->actionDistance_measure, SIGNAL(triggered()),
		this, SLOT(slotRuler()));
	connect(mainWindow.getUi()->actionMaximum_wall_thickness, SIGNAL(triggered()),
		this, SLOT(slotMaximumWallThickness()));
	connect(mainWindow.getUi()->actionPolygon_draw, SIGNAL(triggered()),
		this, SLOT(slotPolygonDraw()));
	connect(mainWindow.getUi()->actionPolygon_draw_series, SIGNAL(triggered()),
		this, SLOT(slotPolygonDrawSeries()));
	connect(mainWindow.getUi()->actionVessel_segmentation, SIGNAL(triggered()),
		this, SLOT(slotVesselSegmentation()));
	connect(mainWindow.getUi()->actionVBD_Smoker, SIGNAL(triggered()),
		this, SLOT(slotVBDSmoker()));




	connect(&mainWindow, SIGNAL(signalImageImportLoad(QList<QStringList>*)),
		&ioManager, SLOT(slotAddToListOfFileNamesAndOpen(QList<QStringList>*)));
	
	// find loading images and overlay
	connect(&ioManager, SIGNAL(signalFinishOpenMultiImages()),
		this, SLOT(slotIOManagerToImageManager()));
	connect(&ioManager, SIGNAL(signalFinishOpenOverlay()),
		this, SLOT(slotOverlayToImageManager()));


	// import and export overlay
	connect(&mainWindow, SIGNAL(signalOverlayImportLoad(QString)), 
		&ioManager, SLOT(slotOpenSegmentation(QString)));
	connect(&mainWindow, SIGNAL(signalOverlayExportSave(QString)),
		&ioManager, SLOT(slotSaveSegmentation(QString)));

	// new overlay
	connect(mainWindow.getUi()->actionNew_segmentation, SIGNAL(triggered()),
		&ioManager, SLOT(slotInitializeOverlay()));

	// change view mode
	connect(mainWindow.getUi()->actionCurved_view, SIGNAL(toggled(bool)),
		this, SLOT(slotCurvedView(bool)));
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		QPushButton* buttons[3] = {
			mainWindow.getViewerWidget(i)->getUi()->pushButtonSigitalView,
			mainWindow.getViewerWidget(i)->getUi()->pushButtonCoronalView,
			mainWindow.getViewerWidget(i)->getUi()->pushButtonAxialView };
		connect(mainWindow.getUi()->actionMulti_planar_view, SIGNAL(triggered()),
			buttons[i%3], SLOT(click()));
		connect(mainWindow.getUi()->actionAll_axial_view, SIGNAL(triggered()),
			buttons[2], SLOT(click()));
	}
	//connect(mainWindow.getUi()->actionAll_axial_view, SIGNAL(triggered()),
	//	this, SLOT(slotAllAxialView()));
	//connect(mainWindow.getUi()->actionMulti_planar_view, SIGNAL(triggered()),
	//	this, SLOT(slotMultiPlanarView()));
	// change image
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		connect(mainWindow.getSelectImgMenu(i), SIGNAL(triggered(QAction*)),
			this, SLOT(slotChangeImage(QAction*)));
		connect(mainWindow.getSelectImgMenu(i), SIGNAL(triggered(QAction*)),
			this, SLOT(slotChangeImage(QAction*)));
	}
	// change slice orientation
	QSignalMapper* sliceOrientationMapperA = new QSignalMapper(this);
	QSignalMapper* sliceOrientationMapperS = new QSignalMapper(this);
	QSignalMapper* sliceOrientationMapperC = new QSignalMapper(this);
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		sliceOrientationMapperA->setMapping(mainWindow.getViewerWidget(i)->getUi()->pushButtonAxialView, i);
		sliceOrientationMapperS->setMapping(mainWindow.getViewerWidget(i)->getUi()->pushButtonSigitalView, i);
		sliceOrientationMapperC->setMapping(mainWindow.getViewerWidget(i)->getUi()->pushButtonCoronalView, i);
		connect(mainWindow.getViewerWidget(i)->getUi()->pushButtonAxialView, SIGNAL(clicked()),
			sliceOrientationMapperA, SLOT(map()));
		connect(mainWindow.getViewerWidget(i)->getUi()->pushButtonSigitalView, SIGNAL(clicked()),
			sliceOrientationMapperS, SLOT(map()));
		connect(mainWindow.getViewerWidget(i)->getUi()->pushButtonCoronalView, SIGNAL(clicked()),
			sliceOrientationMapperC, SLOT(map()));
	}
	connect(sliceOrientationMapperA, SIGNAL(mapped(int)),
		this, SLOT(slotChangeSliceOrientationToXY(int)));
	connect(sliceOrientationMapperS, SIGNAL(mapped(int)),
		this, SLOT(slotChangeSliceOrientationToYZ(int)));
	connect(sliceOrientationMapperC, SIGNAL(mapped(int)),
		this, SLOT(slotChangeSliceOrientationToXZ(int)));

	// surface action
	connect(mainWindow.getUi()->actionTraceball_camera, SIGNAL(triggered()),
		this, SLOT(slotTrackballCamera()));
	connect(mainWindow.getUi()->actionCenter_line, SIGNAL(triggered()),
		this, SLOT(slotCenterLine()));
	connect(mainWindow.getUi()->actionFind_maximum_radius, SIGNAL(triggered()),
		this, SLOT(slotFindMaximumRadius()));
	connect(mainWindow.getUi()->actionPerpendicular_measurement, SIGNAL(triggered()),
		this, SLOT(slotPerpendicularMeasurement()));
	connect(mainWindow.getUi()->actionCurved_navigation, SIGNAL(triggered()),
		this, SLOT(slotCurvedNavigation()));
	connect(mainWindow.getUi()->actionWay_point_centerline, SIGNAL(triggered()),
		this, SLOT(slotWaypoint()));
	connect(mainWindow.getUi()->actionStenosis, SIGNAL(triggered()),
		this, SLOT(slotStenosis()));

	// update btn
	connect(mainWindow.getUi()->updateBtn, SIGNAL(clicked()),
		this, SLOT(slotUpdateSurfaceView()));
	connect(mainWindow.getUi()->updateBtn, SIGNAL(clicked()),
		mainWindow.getUi()->actionTraceball_camera, SLOT(trigger()));
	connect(mainWindow.getUi()->actionUpdate_curved_images, SIGNAL(triggered()),
		this, SLOT(slotInitializeCurvedImage()));

	// Opacity
	//connect(mainWindow.getModuleWidget()->getUi()->opacitySpinBox, SIGNAL(valueChanged(int)),
	//	this, SLOT(slotChangeOpacity(int)));
	//connect(mainWindow.getModuleWidget()->getUi()->labelComboBox, SIGNAL(currentIndexChanged(int)),
	//	this, SLOT(slotUpdateOpacity(int)));

	// Measurement 
	//connect(imageInteractorStyle[DEFAULT_IMAGE]->GetNavigation()->QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
	//	mainWindow.getMeasurementWidget(), SLOT(slotUpdate2DMeasurements()));

	mainWindow.show();
}

Core::~Core()
{

	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		
		imageInteractorStyle[i]->Delete();
		imageInteractorStyle[i] = nullptr;

		
		mainWindow.getViewerWidget(i)->getUi()->qvtkWidget2->GetInteractor()->SetInteractorStyle(nullptr);
		//imageInteractor[i]->Delete();
		//imageInteractor[i] = nullptr;
		
		imageViewers[i]->Delete();
		imageViewers[i] = nullptr;

		//mainWindow.setRenderWindow(i, nullptr);
	}
}

void Core::slotIOManagerToImageManager()
{
	for (int i = 0; i < NUM_OF_IMAGES; ++i) {
			imageManager.setImage(i, ioManager.getListOfImage()[i]);
			imageManager.setDicomIO(i, ioManager.getListOfDicomIOs()[i]);
	}

	// set input to image viewer
	ioManager.slotInitializeOverlay();


	// update selectImgMenus 
	for (int i = 0; i < NUM_OF_IMAGES; ++i) {
		mainWindow.setSelectImgMenuVisible(i, imageManager.getImage(i));
	}
	// reset display extent first, otherwise the slice spin box cannot update right
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageViewers[i]->ResetDisplayExtent();
	}
	// initialization, and trigger the navigation interactorstyle
	mainWindow.initialization();
	const int* extent = imageViewers[DEFAULT_IMAGE]->GetDisplayExtent();
	imageInteractorStyle[DEFAULT_IMAGE]->GetNavigation()->SetCurrentFocalPointWithImageCoordinate(
		(extent[1] - extent[0])*0.5,
		(extent[3] - extent[2])*0.5,
		(extent[5] - extent[4])*0.5
	);

	// clear the memory later, sometimes it will clear too early
	// make no different, if it has not been clear
	//ioManager.clearListOfItkImages();
	//ioManager.clearListOfDicoms();
}

void Core::slotOverlayToImageManager()
{
	imageManager.setOverlay(ioManager.getOverlay());

	// Opacity

	connect(mainWindow.getLabelWidget(), SIGNAL(signalOpacityRequested(int)),
		imageManager.getOverlay(), SLOT(slotRequestOpacity(int)));
	connect(imageManager.getOverlay(), SIGNAL(signalGetRequestedOpacity(double)),
		mainWindow.getLabelWidget(), SLOT(slotSetOpacity(double)));
	connect(mainWindow.getLabelWidget(), SIGNAL(signalOpacityChanged(int, double)),
		imageManager.getOverlay(), SLOT(slotSetOpacity(int, double)));
	connect(mainWindow.getLabelWidget(), SIGNAL(signalOpacityChanged(int, double)),
		this, SLOT(slotRenderALlViewers()));
	
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		slotUpdateImageViewersToCurrent(i);
	}




	// clear the memory later, sometimes it will clear too early
	// make no different, if it has not been clear
	//ioManager.clearOverlay();
}

void Core::slotUpdateMeasurements()
{
	//IADEOverlay* overlay = qobject_cast<IADEOverlay*>(sender());
	IADEOverlay* overlay = dynamic_cast<IADEOverlay*>(sender());
	if (overlay && overlay->Measurements2D.contains(overlay->getCurrentSlice())) {
		mainWindow.getMeasurementWidget()->slotUpdate2DMeasurements(overlay->Measurements2D[overlay->getCurrentSlice()].data());
		mainWindow.getMeasurementWidget()->slotUpdate3DMeasurements(overlay->Measurements3D);
	}
	mainWindow.getMeasurementWidget()->slotUpdateStenosis(
		surfaceInteractorStyle->GetStenosis()->GetStenosisValue()
	);
}


void Core::slotNavigation()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToNavigation();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetNavigation());
}

void Core::slotWindowLevel()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToWindowLevel();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetWindowLevel());
}

void Core::slotThreshold()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToThreshold();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetThreshold());

}

void Core::slotWindowlevelThreshold()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToWindowLevelThreshold();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetWindowLevelThreshold());
}

void Core::slotPaintBrush()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToPaintBrush();
		//imageInteractorStyle[i]->GetPaintBrush()->SetOverlay(imageManager.getOverlay()->getData());
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetPaintBrush());

}

void Core::slotTesting()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToInteractorStyleTesting();
	}
	mainWindow.getModuleWidget()->setWidget(nullptr);

}

void Core::slotSeedsPlacer()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToLumenSeedsPlacer();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetLumenSeedsPlacer());

}

void Core::slotVOI()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToVOI();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetVOI());

}

void Core::slotTubularVOI()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToTubularVOI();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetTubularVOI());

}

void Core::slotRuler()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToRuler();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetRuler());

}

void Core::slotMaximumWallThickness()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToMaximumWallThickness();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetMaximumWallThickness());
}

void Core::slotPolygonDraw()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToPolygonDraw();
	}
	mainWindow.getModuleWidget()->setWidget(nullptr);
}

void Core::slotPolygonDrawSeries()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToPolygonDrawSeries();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetPolygonDrawSeries());

}

void Core::slotVesselSegmentation()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToVesselSegmentation2();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetVesselSegmentation2());
}

void Core::slotTrackballCamera()
{
	surfaceInteractorStyle->SetInteractorStyleTo3DTrackballCamera();
}

void Core::slotCenterLine()
{
	surfaceInteractorStyle->SetInteractorStyleTo3DCenterLine();

}

void Core::slotFindMaximumRadius()
{
	surfaceInteractorStyle->SetInteractorStyleTo3DFindMaximumRadius();
}

void Core::slotPerpendicularMeasurement()
{
	surfaceInteractorStyle->SetInteractorStyleTo3DPerpendicularMeasurement();
}

void Core::slotCurvedNavigation()
{
	surfaceInteractorStyle->SetInteractorStyleTo3DCurvedNavigation();
}

void Core::slotWaypoint()
{
	surfaceInteractorStyle->SetInteractorStyleTo3DWaypoint();
}

void Core::slotStenosis()
{
	surfaceInteractorStyle->SetInteractorStyleTo3DStenosis();
}

void Core::slotVBDSmoker()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToVBDSmoker();
	}
	mainWindow.getModuleWidget()->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetVBDSmoker());
}

void Core::slotInitializeCurvedImage()
{
	dataProcessor.initializeCurved();
}

void Core::slotChangeImage(QAction * action)
{
	QMenu* selectImgMenu = static_cast<QMenu*>(sender());
	unsigned int viewer = 0;
	for (int i = 0; i < MainWindow::NUM_OF_VIEWERS; ++i) {
		if (selectImgMenu == mainWindow.getSelectImgMenu(i)) {
			viewer = i;
			break;
		}
	}
	int image = imageManager.getIndexOfModalityName(action->text());
	slotChangeImage(viewer, image);
}

void Core::slotChangeImage(int viewer, int image)
{
	currentImage[viewer] = image;
	slotUpdateImageViewersToCurrent(viewer);
}

void Core::slotChangeCurved(int viewer, bool curvedFlag)
{
	currentCurved[viewer] = curvedFlag;
	slotUpdateImageViewersToCurrent(viewer);
}

void Core::slotChangeSliceOrientation(int viewer, int sliceOrientation)
{
	currentSliceOrientation[viewer] = sliceOrientation;
	slotUpdateImageViewersToCurrent(viewer);
}

void Core::slotChangeSliceOrientationToYZ(int viewer)
{
	slotChangeSliceOrientation(viewer, ImageViewer::SLICE_ORIENTATION_YZ);
}

void Core::slotChangeSliceOrientationToXZ(int viewer)
{
	slotChangeSliceOrientation(viewer, ImageViewer::SLICE_ORIENTATION_XZ);

}

void Core::slotChangeSliceOrientationToXY(int viewer)
{
	slotChangeSliceOrientation(viewer, ImageViewer::SLICE_ORIENTATION_XY);
}

void Core::slotUpdateImageViewersToCurrent(int viewer)
{

	imageViewers[viewer]->InitializeHeader(imageManager.getModalityName(currentImage[viewer]).toStdString());
	imageViewers[viewer]->SetLookupTable(imageManager.getIADEOverlay()->getLookupTable());
	if (currentCurved[viewer]) {
		imageViewers[viewer]->SetOverlay(imageManager.getCurvedIADEOverlay()->getData());
		imageViewers[viewer]->SetInputData(imageManager.getCurvedImage(currentImage[viewer]));
		// Measurement 
		// tmp fix
		disconnect(imageInteractorStyle[DEFAULT_IMAGE]->GetNavigation()->QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
			imageManager.getIADEOverlay(), SLOT(setCurrentSlice(int)));
		disconnect(imageManager.getIADEOverlay(), SIGNAL(signalUpdatedOverlay()),
			this, SLOT(slotUpdateMeasurements()));
		connect(imageInteractorStyle[DEFAULT_IMAGE]->GetNavigation()->QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
			imageManager.getCurvedIADEOverlay(), SLOT(setCurrentSlice(int)), static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
		connect(imageManager.getCurvedIADEOverlay(), SIGNAL(signalUpdatedOverlay()),
			this, SLOT(slotUpdateMeasurements()), static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
	}
	else
	{
		imageViewers[viewer]->SetOverlay(imageManager.getIADEOverlay()->getData());
		imageViewers[viewer]->SetInputData(imageManager.getImage(currentImage[viewer]));
		// Measurement 
		// tmp fix
		disconnect(imageInteractorStyle[DEFAULT_IMAGE]->GetNavigation()->QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
			imageManager.getCurvedIADEOverlay(), SLOT(setCurrentSlice(int)));
		disconnect(imageManager.getCurvedIADEOverlay(), SIGNAL(signalUpdatedOverlay()),
			this, SLOT(slotUpdateMeasurements()));
		connect(imageInteractorStyle[DEFAULT_IMAGE]->GetNavigation()->QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
			imageManager.getIADEOverlay(), SLOT(setCurrentSlice(int)), static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
		connect(imageManager.getIADEOverlay(), SIGNAL(signalUpdatedOverlay()),
			this, SLOT(slotUpdateMeasurements()), static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));

	}
	imageViewers[viewer]->SetSliceOrientation(currentSliceOrientation[viewer]);
	imageViewers[viewer]->Render();
	//imageViewers[viewer]->InitializeHeader(imageManager.getModalityName(currentImage[viewer]).toStdString());
	//imageViewers[viewer]->Render();

	mainWindow.getViewerWidget(viewer)->setWindowTitle(imageManager.getModalityName(currentImage[viewer]));

}

//void Core::slotMultiPlanarView()
//{
//	slotChangeView(MULTIPLANAR_VIEW);
//}
//
//void Core::slotAllAxialView()
//{
//	slotChangeView(ALL_AXIAL_VIEW);
//}
#include <qmessagebox.h>
#include <vtkPolyData.h>
void Core::slotCurvedView(bool flag)
{
	if (!surfaceViewer->GetCenterline()||surfaceViewer->GetCenterline()->GetNumberOfPoints() < 2) {
		QMessageBox::critical(&mainWindow, QString("No centerline"),
			QString("Please Generate centerline first !"));
		return;
	}
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		if (!imageManager.getCurvedIADEOverlay()) {
			dataProcessor.initializeCurved();
		}
		currentCurved[i] = flag;
		slotUpdateImageViewersToCurrent(i);

	}
}

//void Core::slotChangeView(unsigned int viewMode)
//{
//	if (m_viewMode == viewMode)
//		return;
//	switch (viewMode)
//	{
//	case MULTIPLANAR_VIEW:
//		// MULTIPLANAR_VIEW
//		for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
//			//currentCurved[i] = false;
//			currentSliceOrientation[i] = i % 3;
//			slotUpdateImageViewersToCurrent(i);
//
//		}
//		break;
//	case ALL_AXIAL_VIEW:
//		 // ALL_AXIAL_VIEW
//
//		for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
//			//currentCurved[i] = false;
//			currentSliceOrientation[i] = ImageViewer::SLICE_ORIENTATION_XY;
//			slotUpdateImageViewersToCurrent(i);
//		}
//
//
//			break;
//	//case CURVED_VIEW:
//	//	// CURVED_VIEW
//	//	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
//	//		if (!imageManager.getCurvedIADEOverlay()) {
//	//			dataProcessor.initializeCurved();
//	//		}
//	//		currentCurved[i] = true;
//	//		slotUpdateImageViewersToCurrent(i);
//
//	//	}
//	//	break;
//	default:
//		break;
//	}
//}

void Core::slotUpdateSurfaceView()
{
	// temporary fix for real time updated.
	vtkSmartPointer<vtkImageData> image = vtkSmartPointer<vtkImageData>::New();
	if (currentCurved[DEFAULT_IMAGE]) {
		image->ShallowCopy(imageManager.getCurvedIADEOverlay()->getData());
	}
	else {
		image->ShallowCopy(imageManager.getIADEOverlay()->getData());
	}
	//image->ShallowCopy(imageManager.getIADEOverlay()->getData());
	//surfaceViewer->SetInputData(imageManager.getOverlay()->getData());
	surfaceViewer->SetInputData(image);
	surfaceViewer->SetLookupTable(imageManager.getIADEOverlay()->getLookupTable());
	surfaceViewer->SetCenterline(imageManager.getIADEOverlay()->getCenterLine());
	surfaceViewer->GetRenderer()->ResetCameraClippingRange();
	surfaceViewer->GetRenderer()->ResetCamera();
	surfaceViewer->Render();
}

void Core::slotRenderALlViewers()
{
	imageViewers[0]->Render();
	imageViewers[1]->Render();
	imageViewers[2]->Render();
	surfaceViewer->Render();
}

//void Core::slotChangeOpacity(int opacity)
//{
//	int color = mainWindow.getModuleWidget()->getUi()->labelComboBox->currentIndex();
//	if (imageManager.getOverlay()->getOpacity(color) == opacity) {
//		return;
//	}
//	imageManager.getOverlay()->setOpacity(color, opacity);
//	slotRenderALlViewers();
//}
//
//void Core::slotUpdateOpacity(int opacity)
//{
//	int color = mainWindow.getModuleWidget()->getUi()->labelComboBox->currentIndex();
//	if (imageManager.getOverlay()->getOpacity(color) == opacity) {
//		return;
//	}
//	;
//	mainWindow.getModuleWidget()->getUi()->opacitySpinBox->setValue(imageManager.getOverlay()->getOpacity(color));
//
//}


//void Core::slotTest(bool flag)
//{
//	if (flag == true) {
//		mainWindow.setRenderWindow(0, imageViewers[0]->GetRenderWindow());
//
//	}
//	else {
//		mainWindow.setRenderWindow(0, nullptr);
//
//	}
//	imageViewers[0]->Render();
//}


//void Core::slotIOManagerToImageManager(QList<IOManager::ImageType::Pointer>* images,
//	QList<itk::GDCMImageIO::Pointer>* dicoms) {
//
//	for (int i = 0; i < NUM_OF_IMAGES; ++i) {
//		qDebug() << __LINE__;
//		qDebug() << __FUNCTION__;
//		if (images->at(i)) {
//			imageManager.setImage(i, images->at(i));
//		}
//		if (dicoms->at(i)) {
//			imageManager.setDicomIO(i, dicoms->at(i));
//		}
//	}
//
//
//}
