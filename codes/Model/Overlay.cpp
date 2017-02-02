#include "Overlay.h"

#include <itkVTKImageToImageFilter.h>
#include <itkImageToVTKImageFilter.h>
#include <vtkLookupTable.h>
#include <vtkObjectFactory.h>

vtkStandardNewMacro(Overlay::OverlayImageData);

Overlay::Overlay(QObject * parent)
	:QObject(parent)
{
	m_data = vtkSmartPointer<Overlay::OverlayImageData>::New();
	m_lookupTable = vtkSmartPointer<vtkLookupTable>::New();
	m_lookupTable->SetNumberOfTableValues(1);
	m_lookupTable->Build();
	m_lookupTable->SetTableValue(0, 0, 0, 0, 0);
}

Overlay::Overlay(Overlay::OverlayImageData::itkImageType::Pointer data, QObject * parent)
	:Overlay(parent)
{
	m_data->Graft(data);
}

Overlay::Overlay(Overlay::OverlayImageData * data, QObject * parent)
	: Overlay(parent)
{
	m_data->ShallowCopy(data);
}

Overlay::OverlayImageData * Overlay::getData() const
{
	return m_data;
}

vtkLookupTable * Overlay::getLookupTable() const
{
	return m_lookupTable;
}

void Overlay::OverlayImageData::PrintSelf(ostream & os, vtkIndent indent)
{
	os << indent << "VTK Image information: " << std::endl;
	vtkImageData::PrintSelf(os, indent);
	os << indent << "ITK Image information:" << std::endl;
	m_itkImage->Print(os, 0);
}

void Overlay::OverlayImageData::ShallowCopy(vtkDataObject * dataObject)
{
	OverlayImageData* overlayImageData = OverlayImageData::SafeDownCast(dataObject);
	if (overlayImageData) {
		m_itkImage->Graft(overlayImageData->GetItkImage());
		return;
	}

	vtkImageData *imageData = vtkImageData::SafeDownCast(dataObject);
	if (imageData != NULL)
	{
		vtkImageData::ShallowCopy(imageData);
		updateITKImage();
		return;
	}
}

void Overlay::OverlayImageData::DeepCopy(vtkDataObject * dataObject)
{

	OverlayImageData* overlayImageData = OverlayImageData::SafeDownCast(dataObject);
	if (overlayImageData) {
		m_itkImage->Graft(overlayImageData->GetItkImage());
		return;
	}

	vtkImageData *imageData = vtkImageData::SafeDownCast(dataObject);
	if (imageData != NULL)
	{
		vtkImageData::DeepCopy(imageData);
		updateITKImage();
		return;
	}
}

void Overlay::OverlayImageData::Graft(itkImageType::Pointer dataObject)
{
	m_itkImage->Graft(dataObject);

	updateVTKImage();
}

Overlay::OverlayImageData::itkImageType::Pointer Overlay::OverlayImageData::GetItkImage()
{
	return m_itkImage;
}

Overlay::OverlayImageData::OverlayImageData()
{
	m_itkImage = (itkImageType::New());

	updateVTKImage();
}

Overlay::OverlayImageData::~OverlayImageData()
{
}

void Overlay::OverlayImageData::updateITKImage()
{
	typedef itk::VTKImageToImageFilter<itkImageType> VTKImageToImageType;

	VTKImageToImageType::Pointer vtkImageToImageFilter = VTKImageToImageType::New();
	vtkImageToImageFilter->SetInput(this);
	vtkImageToImageFilter->Update();
	vtkImageToImageFilter->GetOutput()->SetDirection(m_itkImage->GetDirection());
	m_itkImage->Graft(vtkImageToImageFilter->GetOutput());
}

void Overlay::OverlayImageData::updateVTKImage()
{
	typedef itk::ImageToVTKImageFilter<itkImageType> ImageToVTKImageFilter;

	ImageToVTKImageFilter::Pointer imageToVTKImageFilter =
		ImageToVTKImageFilter::New();
	imageToVTKImageFilter->SetInput(m_itkImage);
	imageToVTKImageFilter->Update();
	vtkImageData::ShallowCopy(imageToVTKImageFilter->GetOutput());
}
