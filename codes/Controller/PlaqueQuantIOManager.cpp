#include "PlaqueQuantIOManager.h"

#include "PlaqueQuantOverlay.h"


#include <itkImageFileReader.h>
#include <itkOrientImageFilter.h>
#include <itkImageFileWriter.h>

using itk::OrientImageFilter;
using itk::ImageFileReader;
using itk::ImageFileWriter;


PlaqueQuantIOManager::PlaqueQuantIOManager(QObject * parent)
	:IOManager(parent)
{
}

void PlaqueQuantIOManager::slotInitializeOverlay()
{
	if (listOfImages.size() > 1 && listOfImages[0]) {
		slotInitializeOverlay(listOfImages[0]->GetItkImage());
	}
}

void PlaqueQuantIOManager::slotInitializeOverlay(IVtkImageData::itkImageType::Pointer image)
{
	OverlayImageData::itkImageType::Pointer _overlay
		= OverlayImageData::itkImageType::New();
	_overlay->SetRegions(image->GetLargestPossibleRegion());
	_overlay->SetDirection(image->GetDirection());
	_overlay->SetOrigin(image->GetOrigin());
	_overlay->SetSpacing(image->GetSpacing());
	_overlay->Allocate();
	_overlay->FillBuffer(0);

	overlay = QSharedPointer<Overlay>(new PlaqueQuantOverlay(_overlay));

	emit signalFinishOpenOverlay();
}

void PlaqueQuantIOManager::slotAddToListOfFileNamesAndSave(QList<QStringList>* listOfFileNames)
{
	clearListsOfFileNames();

	for (int i = 0; i < listOfFileNames->size(); ++i) {
		slotAddToListOfFileNames(listOfFileNames->at(i));
	}
	slotSaveMultiImages();
}

void PlaqueQuantIOManager::slotSaveMultiImages()
{
	for (int i = 0; i < listOfCurvedImages.size(); ++i) {
		if (listOfCurvedImages[i]) {
			ImageFileWriter<IVtkImageData::itkImageType>::Pointer writer =
				ImageFileWriter<IVtkImageData::itkImageType >::New();
			writer->SetFileName(listOfFileNames[i].first().toStdString());
			writer->SetInput(listOfCurvedImages[i]->GetItkImage());
			writer->Write();
		}
	}
}

void PlaqueQuantIOManager::slotOpenSegmentation(QString fileName) 
{
	ImageFileReader<OverlayImageData::itkImageType>::Pointer reader =
		ImageFileReader<OverlayImageData::itkImageType>::New();
	reader->SetFileName(fileName.toStdString());
	reader->Update();
	OverlayImageData::itkImageType::Pointer _overlay = reader->GetOutput();

	// using the same m_orientation ITK_COORDINATE_ORIENTATION_RAI
	OrientImageFilter<OverlayImageData::itkImageType, OverlayImageData::itkImageType>::Pointer orienter =
		OrientImageFilter<OverlayImageData::itkImageType, OverlayImageData::itkImageType>::New();
	orienter->UseImageDirectionOn();
	orienter->SetDesiredCoordinateOrientation(
		itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI);
	orienter->SetInput(_overlay);
	orienter->Update();
	_overlay = orienter->GetOutput();

	overlay = QSharedPointer<Overlay>(new PlaqueQuantOverlay(_overlay));

	emit signalFinishOpenOverlay();
}
