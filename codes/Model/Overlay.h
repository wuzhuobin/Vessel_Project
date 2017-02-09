#ifndef __OVERLAY_H__
#define __OVERLAY_H__

//#include "IVtkImageData.h"

#include <qobject.h>

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkCallbackCommand.h>

#include <itkImage.h>

class vtkLookupTable;
class OverlayImageData;
class OverlayModifiedObserver;
class Overlay;

class OverlayImageData :public vtkImageData {

public:
	typedef itk::Image<unsigned char, 3> itkImageType;
	vtkTypeMacro(OverlayImageData, vtkImageData);
	static OverlayImageData* New();
	virtual void PrintSelf(ostream& os, vtkIndent indent);


	// Description:
	// Shallow and Deep copy.
	virtual void ShallowCopy(vtkDataObject *dataObject);
	virtual void DeepCopy(vtkDataObject *dataObject);

	virtual void Graft(itkImageType::Pointer dataObject);

	itkImageType::Pointer GetItkImage();
protected:
	//void operator=(vtkImageData* vtkImageData);
	OverlayImageData();
	~OverlayImageData();

	virtual void updateITKImage();
	virtual void updateVTKImage();

private:
	OverlayImageData(const OverlayImageData&);  // Not implemented.
	void operator=(const OverlayImageData&);  // Not implemented.


											  //vtkSmartPointer<vtkImageData> m_vtkImage;
	itkImageType::Pointer m_itkImage;
	std::string m_modalityName;

};

class OverlayModifiedObserver : public vtkCallbackCommand {
public:
	static OverlayModifiedObserver* New();
	vtkTypeMacro(OverlayModifiedObserver, vtkCallbackCommand);
	Overlay* overlay;
	virtual void Execute(vtkObject *caller, unsigned long eventId, void* callData);
};

class Overlay : public QObject
{
	Q_OBJECT;

public:

	Overlay(QObject* parent = nullptr);
	Overlay(OverlayImageData::itkImageType::Pointer data, QObject* parent = nullptr);
	Overlay(OverlayImageData* data, QObject* parent = nullptr);
	~Overlay();

	OverlayImageData* getData() const;
	vtkLookupTable* getLookupTable() const;

	int getOpacity(int color);
	void setOpacity(int color, int opacity);


protected:
	vtkSmartPointer<OverlayImageData> m_data;
	vtkSmartPointer<vtkLookupTable> m_lookupTable;

	friend class OverlayModifiedObserver;
	vtkSmartPointer<OverlayModifiedObserver> m_modifiedObserver;
	virtual void modified();

};






#endif // !__OVERLAY_H__