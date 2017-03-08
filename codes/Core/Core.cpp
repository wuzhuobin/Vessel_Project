#include "Core.h"

#include "IADEOverlay.h"
#include "ui_MainWindow.h"
#include "ui_ModuleWidget.h"
#include "ui_ViewerWidget.h"


#include <qdebug.h>
#include <QVTKInteractor.h>

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

	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS + MainWindow::NUM_OF_3D_VIEWERS; ++i) {
		if (i % 2) {
			mainWindow.getCentralWidget()->addDockWidget(Qt::TopDockWidgetArea, &viewerWidgets[i]);
		}
		else {
			mainWindow.getCentralWidget()->addDockWidget(Qt::BottomDockWidgetArea, &viewerWidgets[i]);
		}
	}

	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		
		imageViewers[i] = ImageViewer::New();
		imageViewers[i]->SetRenderWindow(viewerWidgets[i].getUi()->qvtkWidget2->GetRenderWindow());
		imageViewers[i]->SetupInteractor(viewerWidgets[i].getUi()->qvtkWidget2->GetInteractor());

		// Never use below method to set the interactorsyle
		//imageInteractorStyle[i]->SetInteractor(imageInteractor[i]);
		imageInteractorStyle[i] = IADEInteractorStyleSwitch::New();
		imageInteractorStyle[i]->SetImageViewer(imageViewers[i]);
		viewerWidgets[i].getUi()->qvtkWidget2->GetInteractor()->SetInteractorStyle(imageInteractorStyle[i]);
		
	}

	//surfaceInteractor = vtkRenderWindowInteractor::New();

	surfaceViewer = SurfaceViewer::New();
	surfaceViewer->SetRenderWindow(viewerWidgets[3].getUi()->qvtkWidget2->GetRenderWindow());
	surfaceViewer->SetupInteractor(viewerWidgets[3].getUi()->qvtkWidget2->GetInteractor());
	surfaceViewer->EnableDepthPeelingOn();
	//surfaceViewer->EnableDepthSortingOn();


	// Never use below method to set the interactorsyle
	//surfaceInteractorStyle[i]->SetInteractor(imageInteractor[i]);
	surfaceInteractorStyle = IADEInteractorStyleSwitch3D::New();
	surfaceInteractorStyle->SetSurfaceViewer(surfaceViewer);
	viewerWidgets[3].getUi()->qvtkWidget2->GetInteractor()->SetInteractorStyle(surfaceInteractorStyle);

	dataProcessor.imageInteractorStyle = imageInteractorStyle;
	dataProcessor.surfaceInteractorStyle = surfaceInteractorStyle;
	dataProcessor.imageManager = &imageManager;

	mainWindow.getUi()->sliceScrollArea->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetNavigation());
	moduleWiget.addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetWindowLevel());
	moduleWiget.addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetPaintBrush());
	moduleWiget.addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetLumenSeedsPlacer());
	moduleWiget.addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetVOI());
	moduleWiget.addWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetVBDSmoker());
	//imageInteractorStyle[DEFAULT_IMAGE]->GetWindowLevel()->show();
	//moduleWiget.setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetWindowLevel());
	
	mainWindow.getUi()->moduleWidgetDockWidget->setWidget(&moduleWiget);


	// connect changing mode
	connect(mainWindow.getUi()->actionTesting, SIGNAL(triggered()),
		this, SLOT(slotTesting()));
	connect(mainWindow.getUi()->actionNavigation, SIGNAL(triggered()),
		this, SLOT(slotNavigation()));
	connect(mainWindow.getUi()->actionWindow_level, SIGNAL(triggered()),
		this, SLOT(slotWindowLevel()));
	connect(mainWindow.getUi()->actionPaint_brush, SIGNAL(triggered()),
		this, SLOT(slotPaintBrush()));
	connect(mainWindow.getUi()->actionSeeds_placer, SIGNAL(triggered()),
		this, SLOT(slotSeedsPlacer()));
	connect(mainWindow.getUi()->acitonVOI_selection, SIGNAL(triggered()),
		this, SLOT(slotVOI()));
	connect(mainWindow.getUi()->actionVBD_Smoker, SIGNAL(triggered()),
		this, SLOT(slotVBDSmoker()));



	//connect(&mainWindow, SIGNAL(signalImageImportInitialize()),
	//	&ioManager, SLOT(slotCleanListsOfFileNames()));
	//connect(&mainWindow, SIGNAL(signalImageImportAdd(QStringList*)),
	//	&ioManager, SLOT(slotAddToListOfFileNames(QStringList*)));
	//connect(&mainWindow, SIGNAL(signalImageImportLoad()),
	//	&ioManager, SLOT(slotOpenMultiImages()));

	connect(&mainWindow, SIGNAL(signalImageImportLoad(QList<QStringList>*)),
		&ioManager, SLOT(slotAddToListOfFileNamesAndOpen(QList<QStringList>*)));
	//connect(&mainWindow, SIGNAL(signalImageImportLoad(QList<QStringList>*)),
	//	&ioManager, SLOT(slotAddToListOfFileNamesAndOpen(QList<QStringList>*)));

	//connect(&ioManager, SIGNAL(signalFinishOpenMultiImages(QList<ImageType::Pointer>*, QList<itk::GDCMImageIO::Pointer>*)),
	//	this, SLOT(slotIOManagerToImageManager(QList<IOManager::ImageType::Pointer>*, QList<itk::GDCMImageIO::Pointer>* dicoms)));
	
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
	connect(mainWindow.getUi()->actionCurved_view, SIGNAL(triggered()),
		this, SLOT(slotCurvedMultiPlanarView()));
	connect(mainWindow.getUi()->actionAll_axial_view, SIGNAL(triggered()),
		this, SLOT(slotAllAxialView()));
	connect(mainWindow.getUi()->actionMulti_planar_view, SIGNAL(triggered()),
		this, SLOT(slotMultiPlanarView()));
	// change image
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		connect(mainWindow.getSelectImgMenu(i), SIGNAL(triggered(QAction*)),
			this, SLOT(slotChangeImage(QAction*)));
	}



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

	// update btn
	connect(mainWindow.getUi()->updateBtn, SIGNAL(clicked()),
		this, SLOT(slotUpdateSurfaceView()));
	connect(mainWindow.getUi()->updateBtn, SIGNAL(clicked()),
		mainWindow.getUi()->actionTraceball_camera, SLOT(trigger()));
	connect(mainWindow.getUi()->actionUpdate_curved_images, SIGNAL(triggered()),
		this, SLOT(slotInitializeCurvedImage()));

	// Opacity
	connect(moduleWiget.getUi()->opacitySpinBox, SIGNAL(valueChanged(int)),
		this, SLOT(slotChangeOpacity(int)));
	connect(moduleWiget.getUi()->labelComboBox, SIGNAL(currentIndexChanged(int)),
		this, SLOT(slotUpdateOpacity(int)));


	mainWindow.show();
}

Core::~Core()
{
	QVTKWidget2* uiViewers[] = {
		mainWindow.getUi()->image1View,
		mainWindow.getUi()->image2View,
		mainWindow.getUi()->image3View,
		mainWindow.getUi()->image4View
	};

	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		
		imageInteractorStyle[i]->Delete();
		imageInteractorStyle[i] = nullptr;

		
		viewerWidgets[i].getUi()->qvtkWidget2->GetInteractor()->SetInteractorStyle(nullptr);
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
		if (imageManager.getImage(i) == nullptr) {
			mainWindow.setSelectImgMenuVisible(i, false);
		}
		else {
			mainWindow.setSelectImgMenuVisible(i, true);
		}
	}
	mainWindow.initialization();
	// clear the memory later, sometimes it will clear too early
	// make no different, if it has not been clear
	//ioManager.clearListOfItkImages();
	//ioManager.clearListOfDicoms();
}

void Core::slotOverlayToImageManager()
{
	imageManager.setOverlay(ioManager.getOverlay());
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		slotUpdateImageViewersToCurrent(i);
	}
	// clear the memory later, sometimes it will clear too early
	// make no different, if it has not been clear
	//ioManager.clearOverlay();
}

void Core::slotCurvedToImageManager()
{
}

void Core::slotNavigation()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToNavigation();
	}
	moduleWiget.setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetNavigation());
}

void Core::slotWindowLevel()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToWindowLevel();
	}
	moduleWiget.setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetWindowLevel());
}

void Core::slotPaintBrush()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToPaintBrush();
		//imageInteractorStyle[i]->GetPaintBrush()->SetOverlay(imageManager.getOverlay()->getData());
	}
	moduleWiget.setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetPaintBrush());

}

void Core::slotTesting()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToInteractorStyleTesting();
	}
	moduleWiget.setWidget(nullptr);

}

void Core::slotSeedsPlacer()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToLumenSeedsPlacer();
	}
	moduleWiget.setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetLumenSeedsPlacer());

}

void Core::slotVOI()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToVOI();
	}
	moduleWiget.setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetVOI());

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

void Core::slotVBDSmoker()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToVBDSmoker();
	}
	moduleWiget.setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetVBDSmoker());
}

void Core::slotInitializeCurvedImage()
{
	dataProcessor.initializeCurved();
}

void Core::slotChangeImage(QAction * action)
{
	QMenu* selectImgMenu = static_cast<QMenu*>(sender());
	int viewer = mainWindow.getSelectImgMenus()->indexOf(selectImgMenu);
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

void Core::slotUpdateImageViewersToCurrent(int viewer)
{
	if (currentCurved[viewer]) {
		imageViewers[viewer]->SetInputDataLayer(imageManager.getCurvedIADEOverlay()->getData());
		imageViewers[viewer]->SetInputData(imageManager.getCurvedImage(currentImage[viewer]));
	}
	else
	{
		imageViewers[viewer]->SetInputDataLayer(imageManager.getIADEOverlay()->getData());
		imageViewers[viewer]->SetInputData(imageManager.getImage(currentImage[viewer]));
	}
	imageViewers[viewer]->SetLookupTable(imageManager.getIADEOverlay()->getLookupTable());
	imageViewers[viewer]->SetSliceOrientation(currentSliceOrientation[viewer]);
	imageViewers[viewer]->Render();
	imageViewers[viewer]->InitializeHeader(imageManager.getModalityName(currentImage[viewer]).toStdString());
	imageViewers[viewer]->Render();
}

void Core::slotMultiPlanarView()
{
	slotChangeView(MULTIPLANAR_VIEW);
}

void Core::slotAllAxialView()
{
	slotChangeView(ALL_AXIAL_VIEW);
}

void Core::slotCurvedMultiPlanarView()
{
	slotChangeView(CURVED_VIEW);
}

void Core::slotChangeView(unsigned int viewMode)
{
	if (m_viewMode == viewMode)
		return;
	switch (viewMode)
	{
	case MULTIPLANAR_VIEW:
		// MULTIPLANAR_VIEW
		for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
			currentCurved[i] = false;
			currentSliceOrientation[i] = i % 3;
			slotUpdateImageViewersToCurrent(i);

		}
		break;
	case ALL_AXIAL_VIEW:
		 // ALL_AXIAL_VIEW

		for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
			currentCurved[i] = false;
			currentSliceOrientation[i] = ImageViewer::SLICE_ORIENTATION_XY;
			slotUpdateImageViewersToCurrent(i);
		}


			break;
	case CURVED_VIEW:
		// CURVED_VIEW
		for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
			if (!imageManager.getCurvedIADEOverlay()) {
				dataProcessor.initializeCurved();
			}
			currentCurved[i] = true;
			slotUpdateImageViewersToCurrent(i);

		}
		break;
	default:
		break;
	}
}

void Core::slotUpdateSurfaceView()
{
	// temporary fix for real time updated.
	vtkSmartPointer<vtkImageData> image = vtkSmartPointer<vtkImageData>::New();
	image->ShallowCopy(imageManager.getOverlay()->getData());
	//surfaceViewer->SetInputData(imageManager.getOverlay()->getData());
	surfaceViewer->SetInputData(image);
	surfaceViewer->SetLookupTable(imageManager.getOverlay()->getLookupTable());
	surfaceViewer->GetRenderer()->ResetCameraClippingRange();
	surfaceViewer->GetRenderer()->ResetCamera();
	surfaceViewer->Render();
}

void Core::RenderALlViewers()
{
	imageViewers[0]->Render();
	imageViewers[1]->Render();
	imageViewers[2]->Render();
	surfaceViewer->Render();
}

void Core::slotChangeOpacity(int opacity)
{
	int color = moduleWiget.getUi()->labelComboBox->currentIndex() + 1;
	if (imageManager.getOverlay()->getOpacity(color) == opacity) {
		return;
	}
	imageManager.getOverlay()->setOpacity(color, opacity);
	RenderALlViewers();
}

void Core::slotUpdateOpacity(int opacity)
{
	int color = moduleWiget.getUi()->labelComboBox->currentIndex() + 1;
	if (imageManager.getOverlay()->getOpacity(color) == opacity) {
		return;
	}
	;
	moduleWiget.getUi()->opacitySpinBox->setValue(imageManager.getOverlay()->getOpacity(color));

}


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
