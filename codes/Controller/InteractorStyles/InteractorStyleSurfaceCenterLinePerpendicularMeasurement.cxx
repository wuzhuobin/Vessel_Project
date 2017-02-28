#include "InteractorStyleSurfaceCenterLinePerpendicularMeasurement.h"

#include "SurfaceViewer.h"

#include <vtkObjectFactory.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkIdList.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkRenderer.h>
#include <vtkLineWidget2.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>

#include <vtkDijkstraGraphInternals.h>

vtkStandardNewMacro(InteractorStyleSurfaceCenterLinePerpendicularMeasurement);

void InteractorStyleSurfaceCenterLinePerpendicularMeasurement::SetCustomEnabled(bool flag)
{
	InteractorStyleSurfaceCenterLineSimpleClipping::SetCustomEnabled(flag);
	if (m_customFlag) {
		InitializeLineWidget2();
		m_surfaceViewer->GetRenderer()->AddActor(m_measurementText);
	}
	else {
		for (int i = 0; i < NUM_OF_HANDLES; ++i) {
			m_handleWidgets[i]->EnabledOff();
			m_handleWidgets[i] = nullptr;
		}
		m_pointLocator = nullptr;
		m_surfaceViewer->GetRenderer()->RemoveActor(m_measurementText);
	}
	

}

InteractorStyleSurfaceCenterLinePerpendicularMeasurement::InteractorStyleSurfaceCenterLinePerpendicularMeasurement()
{

}

InteractorStyleSurfaceCenterLinePerpendicularMeasurement::~InteractorStyleSurfaceCenterLinePerpendicularMeasurement()
{
}

void InteractorStyleSurfaceCenterLinePerpendicularMeasurement::InitializeLineWidget2()
{
	m_lineWidget2 = vtkSmartPointer<vtkLineWidget2>::New();


}

void InteractorStyleSurfaceCenterLinePerpendicularMeasurement::OnKeyPress()
{
	std::string key = this->Interactor->GetKeySym();
	//cout << key << endl;
	//if (key == "Return") {
	//	FindMaximumRadius();
	//}
	//else if (key == "Tab") {
	//	InteractorStyleSurfaceCenterLineSimpleClipping::OnKeyPress();
	//	InitializeHandleWidgets();
	//}
	//else if (key == "space") {
	//	InteractorStyleSurfaceCenterLineSimpleClipping::OnKeyPress();
	//	InitializeHandleWidgets();
	//}
	//else {
	//	InteractorStyleSurfaceCenterLineSimpleClipping::OnKeyPress();
	//}
}


