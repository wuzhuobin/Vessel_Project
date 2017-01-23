#ifndef __IMAGE_MANAGER_H__
#define __IMAGE_MANAGER_H__

#include <qobject.h>
#include "IVtkImageData.h"

class ImageManager: public QObject
{
	Q_OBJECT;

public:
	ImageManager(QObject* parent = nullptr);
	~ImageManager();

private:

};



#endif // !__IMAGE_MANAGER_H__
