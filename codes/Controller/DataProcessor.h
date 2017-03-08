#ifndef __DATA_PROCESSOR_H__
#define __DATA_PROCESSOR_H__

#include <qobject.h>

class IADEInteractorStyleSwitch;
class IADEInteractorStyleSwitch3D;
class IADEImageManager;

class DataProcessor: public QObject
{
	Q_OBJECT;
public:

	DataProcessor(QObject* parent = nullptr);
	~DataProcessor();

	IADEInteractorStyleSwitch** imageInteractorStyle = nullptr;
	IADEInteractorStyleSwitch3D* surfaceInteractorStyle = nullptr;
	IADEImageManager* imageManager = nullptr;

	public slots:

	void initializeCurved();


protected:



private:

};


#endif // !__DATA_PROCESSOR_H__
