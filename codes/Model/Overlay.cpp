#include "Overlay.h"

#include <itkVTKImageToImageFilter.h>
#include <itkImageToVTKImageFilter.h>
#include <vtkLookupTable.h>
#include <vtkObjectFactory.h>

#include <qdebug.h>

vtkStandardNewMacro(OverlayImageData);
vtkStandardNewMacro(OverlayModifiedObserver);

Overlay::Overlay(QObject * parent)
	:QObject(parent)
{
	m_data = vtkSmartPointer<OverlayImageData>::New();

	m_modifiedObserver = vtkSmartPointer<OverlayModifiedObserver>::New();
	m_modifiedObserver->overlay = this;
	m_data->AddObserver(vtkCommand::ModifiedEvent, m_modifiedObserver);

	m_lookupTable = vtkSmartPointer<vtkLookupTable>::New();
	m_lookupTable->SetNumberOfTableValues(1);
	m_lookupTable->Build();
	m_lookupTable->SetTableValue(0, 0, 0, 0, 0);
}

Overlay::Overlay(OverlayImageData::itkImageType::Pointer data, QObject * parent)
	:Overlay(parent)
{
	m_data->Graft(data);
}

Overlay::Overlay(OverlayImageData * data, QObject * parent)
	: Overlay(parent)
{
	m_data->ShallowCopy(data);
}

Overlay::~Overlay()
{
	m_data->RemoveAllObservers();
}

OverlayImageData * Overlay::getData() const
{
	return m_data;
}

vtkLookupTable * Overlay::getLookupTable() const
{
	return m_lookupTable;
}

void Overlay::setOpacity(unsigned int color, unsigned int opacity)
{
	double* value = m_lookupTable->GetTableValue(color);
	value[3] = opacity * 0.01;
	m_lookupTable->SetTableValue(color, value);
	m_lookupTable->Build();
}

unsigned int Overlay::getOpacity(unsigned int color)
{
	double* value = m_lookupTable->GetTableValue(color);
	return value[3]*0.01;
}

void Overlay::modified()
{
	qDebug() << "Modified";
}

void OverlayImageData::PrintSelf(ostream & os, vtkIndent indent)
{
	os << indent << "VTK Image information: " << std::endl;
	vtkImageData::PrintSelf(os, indent);
	os << indent << "ITK Image information:" << std::endl;
	m_itkImage->Print(os, 0);
}

void OverlayImageData::ShallowCopy(vtkDataObject * dataObject)
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

void OverlayImageData::DeepCopy(vtkDataObject * dataObject)
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

void OverlayImageData::Graft(itkImageType::Pointer dataObject)
{
	m_itkImage->Graft(dataObject);

	updateVTKImage();
}

OverlayImageData::itkImageType::Pointer OverlayImageData::GetItkImage()
{
	return m_itkImage;
}

OverlayImageData::OverlayImageData()
{
	m_itkImage = (itkImageType::New());

	updateVTKImage();
}

OverlayImageData::~OverlayImageData()
{
}

void OverlayImageData::updateITKImage()
{
	typedef itk::VTKImageToImageFilter<itkImageType> VTKImageToImageType;

	VTKImageToImageType::Pointer vtkImageToImageFilter = VTKImageToImageType::New();
	vtkImageToImageFilter->SetInput(this);
	vtkImageToImageFilter->Update();
	vtkImageToImageFilter->GetOutput()->SetDirection(m_itkImage->GetDirection());
	m_itkImage->Graft(vtkImageToImageFilter->GetOutput());
}

void OverlayImageData::updateVTKImage()
{
	typedef itk::ImageToVTKImageFilter<itkImageType> ImageToVTKImageFilter;

	ImageToVTKImageFilter::Pointer imageToVTKImageFilter =
		ImageToVTKImageFilter::New();
	imageToVTKImageFilter->SetInput(m_itkImage);
	imageToVTKImageFilter->Update();
	vtkImageData::ShallowCopy(imageToVTKImageFilter->GetOutput());
}

void OverlayModifiedObserver::Execute(vtkObject * caller, unsigned long eventId, void * callData)
{
	overlay->modified();
}
