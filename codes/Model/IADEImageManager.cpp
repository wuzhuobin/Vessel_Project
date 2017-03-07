#include "IADEImageManager.h"

#include "IADEOverlay.h"

#include <itkCastImageFilter.h>

IADEImageManager::IADEImageManager(unsigned int numOfImages, QObject * parent)
	:ImageManager(numOfImages, parent)
{
	setNumOfImages(numOfImages);
}

void IADEImageManager::setNumOfImages(unsigned int num)
{
	ImageManager::setNumOfImages(num);
	for (int i = 0; i < num; ++i) {
		m_curvedImages << nullptr;
	}
}

bool IADEImageManager::setOverlay(IVtkImageData::itkImageType::Pointer image)
{
	typedef itk::CastImageFilter<IVtkImageData::itkImageType, OverlayImageData::itkImageType> CastImageFilter;

	if (image.IsNotNull()) {
		CastImageFilter::Pointer castImageFilter = CastImageFilter::New();
		castImageFilter->SetInput(image);
		castImageFilter->Update();

		m_overlay = QSharedPointer<Overlay>(new IADEOverlay(castImageFilter->GetOutput()));

		return true;
	}
	else
		return false;
}

bool IADEImageManager::setOverlay(OverlayImageData::itkImageType::Pointer image)
{

	if (image.IsNotNull()) {
		m_overlay = QSharedPointer<Overlay>(new IADEOverlay(image));
		return true;
	}
	else
		return false;
}

bool IADEImageManager::setCurvedImage(unsigned int i, IVtkImageData::itkImageType::Pointer image)
{
	if (i >= m_curvedImages.size()) {
		return false;
	}
	if (!image) {
		m_curvedImages[i] = nullptr;
		return false;
	}
	vtkSmartPointer<IVtkImageData> _image =
		vtkSmartPointer<IVtkImageData>::New();
	_image->Graft(image);
	m_curvedImages[i] = _image;
	return true;
}

bool IADEImageManager::setCurvedImage(unsigned int i, vtkImageData * image)
{
	if (i >= m_curvedImages.size()) {
		return false;
	}
	if (!image) {
		m_curvedImages[i] = nullptr;
		return false;
	}
	vtkSmartPointer<IVtkImageData> _image =
		vtkSmartPointer<IVtkImageData>::New();
	_image->ShallowCopy(image);
	m_curvedImages[i] = _image;
	return true;
}

IVtkImageData * IADEImageManager::getCurvedImage(unsigned int i) const
{
	if (i >= m_curvedImages.size()) {
		return nullptr;
	}
	return m_curvedImages[i];
}

IVtkImageData * IADEImageManager::getCurvedImage(QString modalityName) const
{
	unsigned int index = modalityName.indexOf(modalityName);
	if (index < 0) {
		return nullptr;
	}
	return getCurvedImage(index);
}

IADEOverlay * IADEImageManager::getIADEOverlay() const
{
	return static_cast<IADEOverlay*>(m_overlay.data());
}
