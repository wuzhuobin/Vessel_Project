#include "IVtkImageData.h"

#include <vtkObjectFactory.h>

#include <itkVTKImageToImageFilter.h>

vtkStandardNewMacro(IVtkImageData);

void IVtkImageData::PrintSelf(ostream & os, vtkIndent indent)
{
	vtkImageData::PrintSelf(os, indent);
	m_itkImage->Print(os);
}

void IVtkImageData::setVtkInput(vtkImageData * vtkImage)
{
	this->ShallowCopy(vtkImage);
	update();
}

void IVtkImageData::ShallowCopy(vtkDataObject * dataObject)
{
	vtkImageData *imageData = vtkImageData::SafeDownCast(dataObject);

	if (imageData != NULL)
	{
		vtkImageData::ShallowCopy(imageData);
		update();
	}

}

void IVtkImageData::DeepCopy(vtkDataObject * dataObject)
{
	vtkImageData *imageData = vtkImageData::SafeDownCast(dataObject);

	if (imageData != NULL)
	{
		vtkImageData::DeepCopy(imageData);
		update();
	}
}

void IVtkImageData::Graft(itkImageData::Pointer dataObject)
{

}

itkImageData::Pointer IVtkImageData::GetItkOutput()
{
	return m_itkImage;
}

//void IVtkImageData::operator=(vtkImageData * vtkImage)
//{
//	m_vtkImage->ShallowCopy(vtkImage);
//	update();
//}

IVtkImageData::IVtkImageData()
{
	//m_vtkImage = (vtkSmartPointer<vtkImageData>::New());
	//m_vtkImage = this;
	//m_itkImage = (itkImageData::New());


	update();
}

IVtkImageData::~IVtkImageData()
{
}

void IVtkImageData::update()
{
	typedef itk::VTKImageToImageFilter<itkImageData> VTKImageToImageType;

	VTKImageToImageType::Pointer vtkImageToImageFilter = VTKImageToImageType::New();
	vtkImageToImageFilter->SetInput(this);
	vtkImageToImageFilter->Update();
	m_itkImage = vtkImageToImageFilter->GetOutput();

}
