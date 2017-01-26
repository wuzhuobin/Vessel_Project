#ifndef __IMAGE_MANAGER_H__
#define __IMAGE_MANAGER_H__

#include <qobject.h>
#include <qlist.h>

#include <vtkSmartPointer.h>

#include <itkGDCMImageIO.h>

#include "IVtkImageData.h"

class ImageManager: public QObject
{
	Q_OBJECT;

public:
	//typedef itk::GDCMImageIO GDCMImageIO;
	//using itk::GDCMImageIO;

	ImageManager(unsigned int numOfImages = 0, QObject* parent = nullptr);
	~ImageManager();

	void setNumOfImages(unsigned int num);

	bool setImage(unsigned int i, IVtkImageData::itkImageType* image);
	bool setImage(unsigned int i, vtkImageData* image);
	bool setModalityName(unsigned int i, QString name);
	bool setDicomIO(unsigned int i, itk::GDCMImageIO::Pointer dicomIO);

	IVtkImageData* getImage(unsigned int i) const;
	const QString getModalityName(unsigned int i) const;
	const itk::GDCMImageIO::Pointer getDicomIO(unsigned int i) const;

private:

	QList<vtkSmartPointer<IVtkImageData>> m_images;
	QList<QString> m_modalityNames;
	QList<itk::GDCMImageIO::Pointer> m_dicomIOs;


};



#endif // !__IMAGE_MANAGER_H__
