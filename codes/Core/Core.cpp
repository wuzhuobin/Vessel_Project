#include "Core.h"

#include "ImageViewer.h"

#include <qdebug.h>

#include <vtkRenderWindowInteractor.h>
#include <ui_MainWindow.h>

Core::Core(QObject * parent)
	:
	imageManager(NUM_OF_IMAGES, parent),
	ioManager(parent),
	QObject(parent)
{

	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractor[i] = vtkRenderWindowInteractor::New();
		
		imageViewers[i] = ImageViewer::New();
		imageViewers[i]->SetupInteractor(imageInteractor[i]);
		
		imageInteractorStyle[i] = InteractorStyleSwitch::New();
		//imageInteractorStyle[i] = InteractorStyleWindowLevel::New();
		imageInteractorStyle[i]->SetImageViewer(imageViewers[i]);
		
		// Never use below method to set the interactorsyle
		//imageInteractorStyle[i]->SetInteractor(imageInteractor[i]);
		imageInteractor[i]->SetInteractorStyle(imageInteractorStyle[i]);

		mainWindow.setRenderWindow(i, imageViewers[i]->GetRenderWindow());
	}

	// connect changing mode
	connect(mainWindow.geUi()->actionNavigation, SIGNAL(triggered()),
		this, SLOT(slotNavigation()));
	connect(mainWindow.geUi()->actionWindow_level, SIGNAL(triggered()),
		this, SLOT(slotWindowLevel()));


	connect(&mainWindow, SIGNAL(signalImageImportInitialize()),
		&ioManager, SLOT(slotCleanListsOfFileNames()));
	connect(&mainWindow, SIGNAL(signalImageImportAdd(QStringList*)),
		&ioManager, SLOT(slotAddToListOfFileNames(QStringList*)));
	connect(&mainWindow, SIGNAL(signalImageImportLoad()),
		&ioManager, SLOT(slotOpenMultiImages()));

	connect(&mainWindow, SIGNAL(signalImageImportLoad(QList<QStringList>*)),
		&ioManager, SLOT(slotAddToListOfFileNamesAndOpen(QList<QStringList>*)));

	//connect(&ioManager, SIGNAL(signalFinishOpenMultiImages(QList<ImageType::Pointer>*, QList<itk::GDCMImageIO::Pointer>*)),
	//	this, SLOT(slotIOManagerToImageManager(QList<IOManager::ImageType::Pointer>*, QList<itk::GDCMImageIO::Pointer>* dicoms)));
	connect(&ioManager, SIGNAL(signalFinishOpenMultiImages()),
		this, SLOT(slotIOManagerToImageManager()));

	//connect(mainWindow.geUi()->actionNavigation, SIGNAL(toggled(bool)),
	//	this, SLOT(slotTest(bool)));





	mainWindow.show();
}

Core::~Core()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageViewers[i]->Delete();
		mainWindow.setRenderWindow(i, nullptr);
	}
}

void Core::slotIOManagerToImageManager()
{
	for (int i = 0; i < NUM_OF_IMAGES; ++i) {
		qDebug() << __LINE__;
		qDebug() << __FUNCTION__;
		//if (ioManager.getListOfItkImages()->at(i)) {
			imageManager.setImage(i, ioManager.getListOfItkImages()[i]);
		//}
		//if (ioManager.getListOfDicomIOs()->at(i)) {
			imageManager.setDicomIO(i, ioManager.getListOfDicomIOs()[i]);
		//}
	}

	// set input to image viewer
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageViewers[i]->SetInputData(imageManager.getImage(0));
		imageViewers[i]->Render();
		imageViewers[i]->SetSlice(imageViewers[i]->GetSliceMax() / 2);
		imageViewers[i]->Render();
	}
	mainWindow.initialization();
}

void Core::slotNavigation()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetCurrentStyleToNavigation();
	}

}

void Core::slotWindowLevel()
{
	for (int i = 0; i < MainWindow::NUM_OF_2D_VIEWERS; ++i) {
		imageInteractorStyle[i]->SetCurrentStyleToWindowLevel();
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
