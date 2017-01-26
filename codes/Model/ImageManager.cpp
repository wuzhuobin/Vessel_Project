#include "ImageManager.h"

#include "IVtkImageData.h"

ImageManager::ImageManager(unsigned int numOfImages, QObject * parent)
{
	setNumOfImages(numOfImages);
}

ImageManager::~ImageManager()
{
}

void ImageManager::setNumOfImages(unsigned int num)
{
	m_images.clear();
	m_modalityNames.clear();
	m_dicomIOs.clear();
	for (int i = 0; i < num; ++i) {
		m_images << nullptr;
		m_modalityNames << QString();
		m_dicomIOs << nullptr;
	}
}

bool ImageManager::setImage(unsigned int i, IVtkImageData::itkImageType * image)
{
	if (i >= m_images.size()) {
		return false;
	}
	//if (!image) {
	//	return false;
	//}
	vtkSmartPointer<IVtkImageData> _image =
		vtkSmartPointer<IVtkImageData>::New();
	_image->Graft(image);
	_image->Print(cout);
	m_images[i] = _image;
	return true;
}

bool ImageManager::setImage(unsigned int i, vtkImageData * image)
{
	if (i >= m_images.size()) {
		return false;
	}
	//if (!image) {
	//	return false;
	//}
	vtkSmartPointer<IVtkImageData> _image =
		vtkSmartPointer<IVtkImageData>::New();
	_image->ShallowCopy(image);
	m_images[i] = _image;
	return true;
}

bool ImageManager::setModalityName(unsigned int i, QString name)
{
	if (i >= m_modalityNames.size()) {
		return false;
	}
	m_modalityNames[i] = name;
	return true;
}

bool ImageManager::setDicomIO(unsigned int i, itk::GDCMImageIO::Pointer dicomIO)
{
	if (i >= m_dicomIOs.size()) {
		return false;
	}
	m_dicomIOs[i] = dicomIO;
	return true;
}

IVtkImageData * ImageManager::getImage(unsigned int i) const
{
	if (i >= m_images.size()) {
		return nullptr;
	}
	//m_images[i]->Print(cout);
	return m_images[i];
}

const QString ImageManager::getModalityName(unsigned int i) const
{
	if (i >= m_modalityNames.size()) {
		return QString();
	}

	return m_modalityNames[i];
}

const itk::GDCMImageIO::Pointer ImageManager::getDicomIO(unsigned int i) const
{
	if (i >= m_dicomIOs.size()) {
		return nullptr;
	}
	return m_dicomIOs[i];
}
