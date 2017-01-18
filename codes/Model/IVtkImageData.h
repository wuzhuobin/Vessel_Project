#include <vtkImageData.h>

#include <itkImage.h>

typedef itk::Image<double, 3> itkImageData;

class IVtkImageData: public vtkImageData
{

public:
	vtkTypeMacro(IVtkImageData, vtkImageData);
	static IVtkImageData* New();
	void PrintSelf(ostream& os, vtkIndent indent);

	void setVtkInput(vtkImageData* vtkImage);

	// Description:
	// Shallow and Deep copy.
	virtual void ShallowCopy(vtkDataObject *dataObject);
	virtual void DeepCopy(vtkDataObject *dataObject);
	virtual void Graft(itkImageData::Pointer dataObject);

	itkImageData::Pointer GetItkOutput();
	//void operator=(vtkImageData* vtkImageData);


protected: 
	IVtkImageData();
	~IVtkImageData();
	
	virtual void update();

private:
	IVtkImageData(const IVtkImageData&);  // Not implemented.
	void operator=(const IVtkImageData&);  // Not implemented.


	//vtkSmartPointer<vtkImageData> m_vtkImage;
	itkImageData::Pointer m_itkImage;


};
