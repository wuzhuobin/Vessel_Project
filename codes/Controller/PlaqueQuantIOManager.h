#ifndef __PLAQUEQUANT_IO_MANAGER_H__
#define __PLAQUEQUANT_IO_MANAGER_H__

#include "IOManager.h"

class PlaqueQuantIOManager :public IOManager 
{
	Q_OBJECT;

public:
	PlaqueQuantIOManager(QObject* parent = nullptr);

	QList<vtkSmartPointer<IVtkImageData>> listOfCurvedImages;

	public slots:
	virtual void slotOpenSegmentation(QString fileName) override;
	
	virtual void slotInitializeOverlay() override;
	
	virtual	void slotInitializeOverlay(IVtkImageData::itkImageType::Pointer image) override;

	virtual void slotGenerateReport(QString path) override {};

	void slotAddToListOfFileNamesAndSave(QList<QStringList>* listOfFileNames);

	void slotSaveMultiImages();


protected:

};



#endif // !__PLAQUEQUANT_IO_MANAGER_H__
