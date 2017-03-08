#include "AbstractSurfaceCenterLine.h"

#include "SurfaceViewer.h"

#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>


//vtkStandardNewMacro(AbstractSurfaceCenterLine);

vtkSmartPointer<vtkPolyData> AbstractSurfaceCenterLine::m_centerLine = nullptr;

void AbstractSurfaceCenterLine::SetCustomEnabled(bool flag)
{
	AbstractInteractorStyleSurface::SetCustomEnabled(flag);
	if (m_customFlag) {
		if (!m_centerLine) {
			//vtkErrorMacro(<< "please set a centerline before using this interactorStyle");
			m_centerLine = vtkSmartPointer<vtkPolyData>::New();	
			//return;
		}
		if (m_centerLine->GetNumberOfPoints() < 1) {
			CreateCenterLine();
		}
		if (!m_centerLineActor) {
			m_centerLineActor = vtkSmartPointer<vtkActor>::New();
			m_centerLineActor->SetMapper(vtkSmartPointer<vtkPolyDataMapper>::New());
		}
		m_surfaceViewer->GetRenderer()->AddActor(m_centerLineActor);
		VisualizeCenterLine(m_centerLine);
	}
	else {
		if (m_centerLineActor) {
			m_surfaceViewer->GetRenderer()->RemoveActor(m_centerLineActor);
			m_centerLineActor->GetMapper()->SetInputConnection(nullptr);
			m_centerLineActor = nullptr;
		}
	}
	m_surfaceViewer->Render();

}

void AbstractSurfaceCenterLine::SetCenterLine(vtkPolyData* centerLine)
{
	m_centerLine = centerLine;
}

vtkPolyData* AbstractSurfaceCenterLine::GetCenterLine()
{
	return m_centerLine;
}

AbstractSurfaceCenterLine::AbstractSurfaceCenterLine()
{
}

AbstractSurfaceCenterLine::~AbstractSurfaceCenterLine()
{
}

void AbstractSurfaceCenterLine::VisualizeCenterLine(vtkPolyData * centerLine)
{
	vtkPolyDataMapper* mapper = vtkPolyDataMapper::SafeDownCast(m_centerLineActor->GetMapper());
	mapper->SetInputData(centerLine);
	mapper->Update();
	m_surfaceViewer->Render();
}

//void AbstractSurfaceCenterLine::CreateCenterLine()
//{
//}
