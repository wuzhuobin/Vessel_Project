#include "AbstractSurfaceCenterLine.h"

#include "CenterlineSurfaceViewer.h"

#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>


void AbstractSurfaceCenterLine::SetCustomEnabled(bool flag)
{
	AbstractInteractorStyleSurface::SetCustomEnabled(flag);
	if (m_customFlag) {

		if (!GetCenterlineSurfaceViewer()->GetCenterline() ||
			GetCenterlineSurfaceViewer()->GetCenterline()->GetNumberOfPoints() < 1) {
			CreateCenterLine();
			
		}

	}


}

CenterlineSurfaceViewer * AbstractSurfaceCenterLine::GetCenterlineSurfaceViewer()
{
	return reinterpret_cast<CenterlineSurfaceViewer*>(m_viewer);
}

AbstractSurfaceCenterLine::AbstractSurfaceCenterLine()
{
}

AbstractSurfaceCenterLine::~AbstractSurfaceCenterLine()
{
}

