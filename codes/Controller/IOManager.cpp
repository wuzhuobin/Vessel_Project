#include "IOManager.h"

//#include "MyImageManager.h"
#include "ImageRegistration.h"

#include <QFileDialog>
#include <qdebug.h>

#include <itkImageFileReader.h>
#include <itkMetaDataDictionary.h>
#include <itkOrientImageFilter.h>
#include <itkGDCMImageIO.h>
#include <itkImageSeriesReader.h>
#include <itkImageFileWriter.h>

#include <vtkAppendPolyData.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPointData.h>


using itk::GDCMImageIO;
using itk::ImageFileReader;
using itk::ImageSeriesReader;
typedef itk::Image<float, 3> ImageType;
typedef itk::OrientImageFilter<ImageType, ImageType> OrientImageFilter;
typedef itk::Image<float, 3> ImageType;


IOManager::IOManager(QObject* parent)
	:registration(parent), QObject(parent)
{
}

IOManager::~IOManager()
{
}

void IOManager::enableRegistration(bool flag)
{
	this->registrationFlag = flag;
}

void IOManager::slotAddToListOfFileNamesAndOpen(QList<QStringList>* listOfFileNames)
{
	clearListsOfFileNames();
	
	for (int i = 0; i < listOfFileNames->size(); ++i) {
		slotAddToListOfFileNames(listOfFileNames->at(i));
	}

	slotOpenMultiImages();
}

void IOManager::slotAddToListOfFileNames(QStringList fileNames)
{
	this->listOfFileNames.append(fileNames);
}

void IOManager::slotAddToListOfFileNames(QStringList * fileNames)
{
	slotAddToListOfFileNames(*fileNames);
}

void IOManager::clearListsOfFileNames()
{
	this->listOfFileNames.clear();
}

const QList<QStringList> IOManager::getListOfFileNames() const
{
	return this->listOfFileNames;
}

const QList<ImageType::Pointer> IOManager::getListOfItkImages() const
{
	return this->listOfItkImages;
}

void IOManager::clearListOfItkImages()
{
	this->listOfItkImages.clear();
}

const QList<GDCMImageIO::Pointer> IOManager::getListOfDicomIOs() const
{
	return this->listOfDicomIOs;
}

void IOManager::clearListOfDicoms()
{
	this->listOfDicomIOs.clear();
}

const ImageType::Pointer IOManager::getOverlay() const
{
	return overlay;
}

void IOManager::clearOverlay()
{
	overlay = nullptr;
}

bool IOManager::loadImageData(QStringList fileNames)
{

	ImageType::Pointer _itkImage = nullptr;
	GDCMImageIO::Pointer _dicomIO = nullptr;

	if (fileNames.isEmpty() || fileNames.value(0) == QString()) {
		_itkImage = nullptr;
		_dicomIO = nullptr;
	}
	// load Nifti Data
	else if (fileNames.size() == 1) {
		ImageFileReader<ImageType>::Pointer reader;

		try {
			reader = ImageFileReader<ImageType>::New();
			reader->SetFileName(fileNames[0].toStdString());
			reader->SetImageIO(_dicomIO);
			reader->Update();
			_itkImage = reader->GetOutput();

		}
		catch (itk::ImageFileReaderException& e) {
			qDebug() << "Fail to read Dicom header";
			qDebug() << e.what();
			qDebug() << "Description: " << e.GetDescription();
			reader = ImageFileReader<ImageType>::New();
			reader->SetFileName(fileNames[0].toStdString());
			reader->Update();
			_itkImage = reader->GetOutput();
		}
	}
	// Load Dicom Data
	else {
		std::vector<std::string> _fileNames;
		for (QStringList::const_iterator constIterator = fileNames.constBegin();
			constIterator != fileNames.constEnd(); ++constIterator) {
			_fileNames.push_back(constIterator->toStdString());
		}
		_dicomIO = GDCMImageIO::New();

		ImageSeriesReader<ImageType>::Pointer reader =
			ImageSeriesReader<ImageType>::New();
		reader->SetFileNames(_fileNames);
		reader->SetImageIO(_dicomIO);
		reader->Update();
		_itkImage = reader->GetOutput();

	}
	if (_itkImage.IsNotNull()) {
		// using the same m_orientation ITK_COORDINATE_ORIENTATION_RAI
		OrientImageFilter::Pointer orienter =
			OrientImageFilter::New();
		orienter->UseImageDirectionOn();
		orienter->SetDesiredCoordinateOrientation(
			itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI);
		orienter->SetInput(_itkImage);
		orienter->Update();
		_itkImage = orienter->GetOutput();
		// Image Registration
		if (this->registrationFlag && 
			this->listOfItkImages.size() > 0 && 
			this->listOfItkImages[0].IsNotNull()) {
			_itkImage = imageAlignment(this->listOfItkImages[0], _itkImage);
		}

	}
	listOfItkImages << _itkImage;
	listOfDicomIOs << _dicomIO;
	return true;
}

IOManager::ImageType::Pointer IOManager::imageAlignment(ImageType::Pointer alignedTo, ImageType::Pointer toBeAligned)
{
	//this->registration.SetFixedImage(alignedTo);
	//this->registration.SetMovingImage(toBeAligned);
	//this->registration.Update();

	//return registration.GetOutput();
	return nullptr;
}

void IOManager::slotOpenMultiImages()
{
	// initialize modality names
	//this->myImageManager->allClear();
	//this->myImageManager->listOfModalityNames += "CUBE T1";
	//this->myImageManager->listOfModalityNames += "CUBE T2";
	//this->myImageManager->listOfModalityNames += "CUBE T1+C";
	//this->myImageManager->listOfModalityNames += "2D DIR/QIR";
	//this->myImageManager->listOfModalityNames += "MPRAGE";

	slotCleanImagesAndDicomIOs();

	bool _flag = this->registrationFlag;
	for (QList<QStringList>::const_iterator cit = this->listOfFileNames.constBegin();
		cit != this->listOfFileNames.constEnd(); ++cit) {
		// Never do registration for the first image
		if (cit == listOfFileNames.constBegin()) {
			this->registrationFlag = false;
		}
		else {
			this->registrationFlag = _flag;
		}
		loadImageData(*cit);
		//slotOpenOneImage(*cit);
	}
	//this->myImageManager->overlay->Initialize(
	//	this->myImageManager->listOfVtkImages[0]);

	emit signalFinishOpenMultiImages();
	emit signalFinishOpenMultiImages(&this->listOfItkImages, &this->listOfDicomIOs);
}

void IOManager::slotOpenOneImage(QStringList fileNames)
{
	bool _flag = this->registrationFlag;
	this->registrationFlag = false;
	loadImageData(fileNames);
	this->registrationFlag = _flag;
	emit signalFinishOpenOneImage();
}

void IOManager::slotCleanImagesAndDicomIOs()
{
	this->listOfDicomIOs.clear();
	this->listOfItkImages.clear();
}

void IOManager::slotInitializeOverlay()
{
	if (listOfItkImages.size() > 1 && listOfItkImages[0].IsNotNull()){
		slotInitializeOverlay(listOfItkImages[0]);
	}
}

void IOManager::slotInitializeOverlay(ImageType::Pointer image)
{
	overlay = ImageType::New();
	overlay->SetRegions(image->GetLargestPossibleRegion());
	overlay->SetDirection(image->GetDirection());
	overlay->SetOrigin(image->GetOrigin());
	overlay->SetSpacing(image->GetSpacing());
	overlay->Allocate();
	overlay->FillBuffer(0);

	emit signalFinishOpenOverlay();
}

//void IOManager::slotOpenSegmentationWithDiaglog()
//{
//	QString path = QFileDialog::getOpenFileName(dynamic_cast<QWidget*>(this->parent()),
//		QString(tr("Open Segmentation")), ".", tr("NlFTI Images (*.nii)"));
//	if (path.isEmpty()) return;
//	this->slotOpenSegmentation(path);
//	emit finishOpenSegmentation();
//}
//
void IOManager::slotOpenSegmentation(QString fileName)
{
	overlay = ImageType::New();
	ImageFileReader<ImageType>::Pointer reader =
		ImageFileReader<ImageType>::New();
	reader->SetFileName(fileName.toStdString());
	reader->Update();
	overlay = reader->GetOutput();

	// using the same m_orientation ITK_COORDINATE_ORIENTATION_RAI
	OrientImageFilter::Pointer orienter =
		OrientImageFilter::New();
	orienter->UseImageDirectionOn();
	orienter->SetDesiredCoordinateOrientation(
		itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI);
	orienter->SetInput(overlay);
	orienter->Update();
	overlay = orienter->GetOutput();

	emit signalFinishOpenOverlay();

}
void IOManager::slotSaveSegmentation(QString path)
{
	slotSaveSegmentation(overlay2, path);
}
void IOManager::slotSaveSegmentation(OverlayType::Pointer input, QString path)
{
	typedef itk::ImageFileWriter<OverlayType> ImageFileWriter;
		ImageFileWriter::Pointer writer =
			ImageFileWriter::New();
		writer->SetInput(input);
		writer->SetFileName(path.toStdString().c_str());
		writer->Write();
}

//
//void IOManager::slotSaveSegmentaitonWithDiaglog()
//{
//	QString path = QFileDialog::getSaveFileName(dynamic_cast<QWidget*>(this->parent()), 
//		QString(tr("Save Segmentation")), ".", tr("NlFTI Images (*.nii)"));
//	if (path.isEmpty())	return;
//	slotSaveSegmentation(path);
//	
//}
//
//void IOManager::slotSaveSegmentation(QString path)
//{
//	ImageFileWriter<Overlay::OverlayImageType>::Pointer writer =
//		ImageFileWriter<Overlay::OverlayImageType>::New();
//	writer->SetInput(this->myImageManager->overlay->GetITKOutput<ImageType>(
//		this->myImageManager->listOfItkImages[0]));
//	writer->SetFileName(path.toStdString().c_str());
//	writer->Write();
//}
//
//void IOManager::slotSaveContourWithDiaglog()
//{
//	QString path = QFileDialog::getSaveFileName(dynamic_cast<QWidget*>(this->parent()),
//		QString(tr("Save Contours")), ".", tr("Serial vtkPolyData(*.vtp)"));
//	if (path.isEmpty())	return;
//	slotSaveContour(path);
//}
//
//void IOManager::slotSaveContour(QString fileName)
//{
//	vtkSmartPointer<vtkAppendPolyData> append =
//		vtkSmartPointer<vtkAppendPolyData>::New();
//
//
//	Overlay* overlay = this->myImageManager->getOverlay();
//	QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>*  contours[2] =
//	{ overlay->GetLumenPolyData(), overlay->GetVesselWallPolyData() };
//	for (int i = 0; i < 2; ++i) {
//
//
//
//		for (QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>::const_iterator cit1 =
//			contours[i]->cbegin(); cit1 != contours[i]->cend();++cit1) {
//			
//			QList<vtkSmartPointer<vtkPolyData>>* _list = cit1.value();
//			for (QList<vtkSmartPointer<vtkPolyData>>::const_iterator cit2 = _list->cbegin();
//				cit2 != _list->cend(); ++cit2) {
//
//				// set point data for classification
//				vtkSmartPointer<vtkUnsignedCharArray> scalars =
//					vtkSmartPointer<vtkUnsignedCharArray>::New();
//				scalars->SetNumberOfValues((*cit2)->GetNumberOfPoints());
//				scalars->SetNumberOfComponents(1);
//				scalars->FillComponent(0, i + 1);
//
//				(*cit2)->GetPointData()->SetScalars(scalars);
//				append->AddInputData(*cit2);
//			}
//
//
//		}
//	}
//	append->Update();
//
//	vtkSmartPointer<vtkXMLPolyDataWriter> writer =
//		vtkSmartPointer<vtkXMLPolyDataWriter>::New();
//	writer->SetInputConnection(append->GetOutputPort());
//	writer->SetFileName(fileName.toStdString().c_str());
//	writer->Write();
//}
