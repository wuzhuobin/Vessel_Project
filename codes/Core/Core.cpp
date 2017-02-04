#include "Core.h"

#include "ImageViewer.h"
#include "QInteractorStyleNavigation.h"
#include "Overlay.h"
#include "ui_MainWindow.h"


#include <qdebug.h>

#include <vtkRenderWindowInteractor.h>
#include <vtkLookupTable.h>

Core::Core(QObject * parent)
	:
	imageManager(NUM_OF_IMAGES, parent),
	ioManager(parent),
	QObject(parent)
{

	imageManager.setModalityName(0, "T2 Image");
	imageManager.setModalityName(1, "MRA Image");
	mainWindow.addModalityNames("T2 Image");
	mainWindow.addModalityNames("MRA Image");


	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractor[i] = vtkRenderWindowInteractor::New();
		
		imageViewers[i] = ImageViewer::New();
		imageViewers[i]->SetupInteractor(imageInteractor[i]);
		
		imageInteractorStyle[i] = IADEInteractorStyleSwitch::New();
		//imageInteractorStyle[i] = InteractorStyleWindowLevel::New();
		imageInteractorStyle[i]->SetImageViewer(imageViewers[i]);
		
		// Never use below method to set the interactorsyle
		//imageInteractorStyle[i]->SetInteractor(imageInteractor[i]);
		imageInteractor[i]->SetInteractorStyle(imageInteractorStyle[i]);

		mainWindow.setRenderWindow(i, imageViewers[i]->GetRenderWindow());
	}
	mainWindow.geUi()->sliceScrollArea->setWidget(imageInteractorStyle[DEFAULT_IMAGE]->GetNavigation());


	// connect changing mode
	connect(mainWindow.geUi()->actionNavigation, SIGNAL(triggered()),
		this, SLOT(slotNavigation()));
	connect(mainWindow.geUi()->actionWindow_level, SIGNAL(triggered()),
		this, SLOT(slotWindowLevel()));


	//connect(&mainWindow, SIGNAL(signalImageImportInitialize()),
	//	&ioManager, SLOT(slotCleanListsOfFileNames()));
	//connect(&mainWindow, SIGNAL(signalImageImportAdd(QStringList*)),
	//	&ioManager, SLOT(slotAddToListOfFileNames(QStringList*)));
	//connect(&mainWindow, SIGNAL(signalImageImportLoad()),
	//	&ioManager, SLOT(slotOpenMultiImages()));

	connect(&mainWindow, SIGNAL(signalImageImportLoad(QList<QStringList>*)),
		&ioManager, SLOT(slotAddToListOfFileNamesAndOpen(QList<QStringList>*)));
	connect(&mainWindow, SIGNAL(signalImageImportLoad(QList<QStringList>*)),
		&ioManager, SLOT(slotAddToListOfFileNamesAndOpen(QList<QStringList>*)));

	//connect(&ioManager, SIGNAL(signalFinishOpenMultiImages(QList<ImageType::Pointer>*, QList<itk::GDCMImageIO::Pointer>*)),
	//	this, SLOT(slotIOManagerToImageManager(QList<IOManager::ImageType::Pointer>*, QList<itk::GDCMImageIO::Pointer>* dicoms)));
	connect(&ioManager, SIGNAL(signalFinishOpenMultiImages()),
		this, SLOT(slotIOManagerToImageManager()));
	//connect(&ioManager, SIGNAL(signalFinishOpenMultiImages()),
	//	&ioManager, SLOT(slotInitializeOverlay()));
	connect(&ioManager, SIGNAL(signalFinishOpenOverlay()),
		this, SLOT(slotOverlayToImageManager()));

	//connect(mainWindow.geUi()->actionNavigation, SIGNAL(toggled(bool)),
	//	this, SLOT(slotTest(bool)));





	mainWindow.show();
}

Core::~Core()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		
		imageInteractorStyle[i]->Delete();
		imageInteractorStyle[i] = nullptr;

		imageInteractor[i]->SetInteractorStyle(nullptr);
		imageInteractor[i]->Delete();
		imageInteractor[i] = nullptr;
		
		imageViewers[i]->Delete();
		imageViewers[i] = nullptr;

		mainWindow.setRenderWindow(i, nullptr);
	}
}

void Core::slotIOManagerToImageManager()
{
	for (int i = 0; i < NUM_OF_IMAGES; ++i) {
			imageManager.setImage(i, ioManager.getListOfItkImages()[i]);
			imageManager.setDicomIO(i, ioManager.getListOfDicomIOs()[i]);
	}
	ioManager.slotInitializeOverlay();
	// set input to image viewer
	slotMultiPlanarView();

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
}

void Core::slotOverlayToImageManager()
{
	imageManager.setOverlay(ioManager.getOverlay());
}

void Core::slotNavigation()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToNavigation();
	}

}

void Core::slotWindowLevel()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetInteractorStyleToWindowLevel();
	}
}

void Core::slotMultiPlanarView()
{
	slotChangeView(MULTIPLANAR_VIEW);
}

void Core::slotAllAxialView()
{
	slotChangeView(ALL_AXIAL_VIEW);
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
			// Change input to same image, default 0
			// SetupInteractor should be ahead of InitializeHeader
			imageViewers[i]->SetInputData(imageManager.getImage(DEFAULT_IMAGE));
			// Overlay settings
			imageViewers[i]->SetInputDataLayer(imageManager.getOverlay()->getData());
			imageViewers[i]->SetLookupTable(imageManager.getOverlay()->getLookupTable());
			
			imageViewers[i]->SetSliceOrientation(i);
			imageViewers[i]->Render();
			imageViewers[i]->InitializeHeader(imageManager.getModalityName(DEFAULT_IMAGE).toStdString());
			imageViewers[i]->Render();
			// else only change input and viewer m_orientation
			//imageViewers[i]->GetRenderWindow()->GetInteractor()->Enable();
			// Show view props for overlay
			//m_2DimageViewer[i]->SetAllBlack(false);

		}
		break;
	case ALL_AXIAL_VIEW:
		 // ALL_AXIAL_VIEW
			// i1 for looping all 5 vtkImage, while i2 for looping all 3 m_2DimageViewer
			for (int i1 = 0, i2 = 0; i2 < MainWindow::NUM_OF_2D_VIEWERS; ++i2)
			{
				for (; i1 < NUM_OF_IMAGES && i2 < MainWindow::NUM_OF_2D_VIEWERS;
					++i1) {
					// skip the NULL image
					if (this->imageManager.getImage(i1) != nullptr) {
						// SetupInteractor should be ahead of InitializeHeader
						this->imageViewers[i2]->SetInputData(
							this->imageManager.getImage(i1));
						this->imageViewers[i2]->SetSliceOrientation(ImageViewer::SLICE_ORIENTATION_XY);
						this->imageViewers[i2]->Render();
						this->imageViewers[i2]->InitializeHeader(imageManager.getModalityName(i2).toStdString());

						++i2;
					}
				}
				if (i1 >= NUM_OF_IMAGES && i2 < MainWindow::NUM_OF_2D_VIEWERS) {
					//this->imageViewers[i2]->GetRenderWindow()->GetInteractor()->Disable();
					//// disable view props
					//imageViewers[i2]->SetAllBlack(true);

				}

			}

		break;
	default:
		break;
	}
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


void Core::slotIOManagerToImageManager(QList<IOManager::ImageType::Pointer>* images,
	QList<itk::GDCMImageIO::Pointer>* dicoms) {

	for (int i = 0; i < NUM_OF_IMAGES; ++i) {
		qDebug() << __LINE__;
		qDebug() << __FUNCTION__;
		if (images->at(i)) {
			imageManager.setImage(i, images->at(i));
		}
		if (dicoms->at(i)) {
			imageManager.setDicomIO(i, dicoms->at(i));
		}
	}


}
