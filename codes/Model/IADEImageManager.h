#ifndef __IADE_IMAGE_MANAGER_H__
#define __IADE_IMAGE_MANAGER_H__

#include "ImageManager.h"

class IADEOverlay;

class IADEImageManager: public ImageManager
{
	Q_OBJECT;

public:
	IADEImageManager(unsigned int numOfImages = 0, QObject* parent = nullptr);

	virtual void setNumOfImages(unsigned int num) override;

	virtual bool setOverlay(IVtkImageData::itkImageType::Pointer image);
	virtual bool setOverlay(OverlayImageData::itkImageType::Pointer image);
	bool setCurvedImage(unsigned int i, IVtkImageData::itkImageType::Pointer image);
	bool setCurvedImage(unsigned int i, vtkImageData* image);
	IVtkImageData* getCurvedImage(unsigned int i) const;
	IVtkImageData* getCurvedImage(QString modalityName) const;
	IADEOverlay* getIADEOverlay() const;

protected:
	QList<vtkSmartPointer<IVtkImageData>> m_curvedImages;
	QSharedPointer<Overlay> m_curvedOverlay;



private:

};



#endif // !__IADE_IMAGE_MANAGER_H__
