#ifndef __OVERLAY_H__
#define __OVERLAY_H__

//#include "IVtkImageData.h"

#include <qobject.h>

#include <vtkSmartPointer.h>
#include <vtkImageData.h>

#include <itkImage.h>

class vtkLookupTable;

class Overlay: public QObject
{
	Q_OBJECT;

public: 
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
public:

	Overlay(QObject* parent = nullptr);
	Overlay(OverlayImageData::itkImageType::Pointer data, QObject* parent = nullptr);
	Overlay(OverlayImageData* data, QObject* parent = nullptr);

	OverlayImageData* getData() const;
	vtkLookupTable* getLookupTable() const;

private:
	vtkSmartPointer<OverlayImageData> m_data;
	vtkSmartPointer<vtkLookupTable> m_lookupTable;




};






#endif // !__OVERLAY_H__