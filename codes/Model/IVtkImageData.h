#ifndef __IVTK_IMAGE_DATA_H__
#define __IVTK_IMAGE_DATA_H__

#include <vtkImageData.h>

#include <itkImage.h>

typedef itk::Image<float, 3> itkImageType;

class IVtkImageData : public vtkImageData
{

public:
	vtkTypeMacro(IVtkImageData, vtkImageData);
	static IVtkImageData* New();
	void PrintSelf(ostream& os, vtkIndent indent);


	// Description:
	// Shallow and Deep copy.
	virtual void ShallowCopy(vtkDataObject *dataObject);
	virtual void DeepCopy(vtkDataObject *dataObject);
	virtual void Graft(itkImageType::Pointer dataObject);

	itkImageType::Pointer GetItkImage();
	//void operator=(vtkImageData* vtkImageData);


protected:
	IVtkImageData();
	~IVtkImageData();

	virtual void updateITKImage();
	virtual void updateVTKImage();

private:
	IVtkImageData(const IVtkImageData&);  // Not implemented.
	void operator=(const IVtkImageData&);  // Not implemented.


										   //vtkSmartPointer<vtkImageData> m_vtkImage;
	itkImageType::Pointer m_itkImage;


};


#endif // !__IVTK_IMAGE_DATA_H__