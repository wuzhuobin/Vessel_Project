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
	for (int i = 0; i < num; ++i) {
		m_images << nullptr;
	}
	for (int i = 0; i < num; ++i) {
		m_modalityNames << QString();
	}
}

bool ImageManager::setImage(unsigned int i, IVtkImageData * image)
{
	if (i >= m_images.size()) {
		return false;
	}
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

const IVtkImageData * ImageManager::getImage(unsigned int i) const
{
	if (i >= m_images.size()) {
		return nullptr;
	}
	return m_images[i];
}

const QString ImageManager::getModalityName(unsigned int i) const
{
	if (i >= m_modalityNames.size()) {
		return QString();
	}

	return m_modalityNames[i];
}
