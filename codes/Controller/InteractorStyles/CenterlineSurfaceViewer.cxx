#include "CenterlineSurfaceViewer.h"

#include <vtkObjectFactory.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
vtkStandardNewMacro(CenterlineSurfaceViewer);

void CenterlineSurfaceViewer::PrintSelf(ostream & os, vtkIndent indent)
{
	SurfaceViewer::PrintSelf(os, indent);
}

void CenterlineSurfaceViewer::SetCenterline(vtkPolyData * centerline)
{
}

vtkPolyData * CenterlineSurfaceViewer::GetCenterline()
{
	return nullptr;
}

CenterlineSurfaceViewer::CenterlineSurfaceViewer()
{
	this->CenterlineActor = vtkActor::New();
	this->CenterlineActor->VisibilityOff();
	this->CenterlineMapper = vtkPolyDataMapper::New();

	if (this->CenterlineActor && this->CenterlineMapper) {
		// for make sure the Actor is not opaque
		this->CenterlineActor->SetMapper(this->CenterlineMapper);
	}
	this->InstallPipeline();
}

CenterlineSurfaceViewer::~CenterlineSurfaceViewer()
{
	if (this->CenterlineMapper)
	{
		this->CenterlineMapper->Delete();
		this->CenterlineMapper = nullptr;
	}
	if (this->CenterlineActor)
	{
		this->CenterlineActor->Delete();
		this->CenterlineActor = nullptr;
	}
}

void CenterlineSurfaceViewer::InstallPipeline()
{
	SurfaceViewer::InstallPipeline();

}

void CenterlineSurfaceViewer::UnInstallPipeline()
{
	if (this->CenterlineActor)
	{
		this->CenterlineActor->GetMapper()->SetInputConnection(nullptr);
	}
}
