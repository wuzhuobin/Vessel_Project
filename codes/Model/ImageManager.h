#ifndef __IMAGE_MANAGER_H__
#define __IMAGE_MANAGER_H__

#include <qobject.h>
#include <qlist.h>

#include <vtkSmartPointer.h>

#include "IVtkImageData.h"

class IVtkImageData;


class ImageManager: public QObject
{
	Q_OBJECT;

public:
	ImageManager(unsigned int numOfImages = 0, QObject* parent = nullptr);
	~ImageManager();

	void setNumOfImages(unsigned int num);


	bool setImage(unsigned int i, IVtkImageData* image);
	bool setModalityName(unsigned int i, QString name);

	const IVtkImageData* getImage(unsigned int i) const;
	const QString getModalityName(unsigned int i) const;


private:

	QList<vtkSmartPointer<IVtkImageData>> m_images;
	QList<QString> m_modalityNames;


};



#endif // !__IMAGE_MANAGER_H__
