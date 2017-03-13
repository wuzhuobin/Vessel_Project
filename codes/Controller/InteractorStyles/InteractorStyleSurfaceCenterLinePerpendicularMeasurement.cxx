#include "InteractorStyleSurfaceCenterLinePerpendicularMeasurement.h"

#include "SurfaceViewer.h"
#include "PerpendicularMeasurementLineWidget.h"

#include <vtkObjectFactory.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkIdList.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkRenderer.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkLineRepresentation.h>
#include <vtkPointHandleRepresentation3D.h>



vtkStandardNewMacro(InteractorStyleSurfaceCenterLinePerpendicularMeasurement);

void InteractorStyleSurfaceCenterLinePerpendicularMeasurement::SetCustomEnabled(bool flag)
{
	InteractorStyleSurfaceCenterLineSimpleClipping::SetCustomEnabled(flag);
	if (m_customFlag) {
		InitializeLinePerpendicularMeasurementWidget();
		GetSurfaceViewer()->GetRenderer()->AddActor(m_measurementText);
	}
	else {
		m_lineWidget2->SetInteractor(nullptr);
		m_lineWidget2->EnabledOff();
		m_lineWidget2 = nullptr;

		//m_distanceWidget->SetInteractor(nullptr);
		//m_distanceWidget->EnabledOff();
		//m_distanceWidget = nullptr;

		GetSurfaceViewer()->GetRenderer()->RemoveActor(m_measurementText);
	}
	

}

void InteractorStyleSurfaceCenterLinePerpendicularMeasurement::FindMaxiMumPerpendicularDistance()
{
	double pos1[3], pos2[3];
	this->m_lineWidget2->GetLineRepresentation()->GetPoint1WorldPosition(pos1);
	this->m_lineWidget2->GetLineRepresentation()->GetPoint2WorldPosition(pos2);
	//for(vtkIdType id = 0; i < m_centerLine->GetNumberOfPoints())
}

InteractorStyleSurfaceCenterLinePerpendicularMeasurement::InteractorStyleSurfaceCenterLinePerpendicularMeasurement()
{
	m_measurementText =
		vtkSmartPointer<vtkTextActor>::New();
	m_measurementText->SetPosition2(10, 40);
	m_measurementText->GetTextProperty()->SetFontSize(24);
	m_measurementText->GetTextProperty()->SetColor(1.0, 0.0, 0.0);
}

InteractorStyleSurfaceCenterLinePerpendicularMeasurement::~InteractorStyleSurfaceCenterLinePerpendicularMeasurement()
{
}

void InteractorStyleSurfaceCenterLinePerpendicularMeasurement::InitializeLinePerpendicularMeasurementWidget()
{
	vtkSmartPointer<vtkPolygonalSurfacePointPlacer> pointPlacer =
		vtkSmartPointer<vtkPolygonalSurfacePointPlacer>::New();
	pointPlacer->AddProp(m_centerLineActor);


	m_lineWidget2 = vtkSmartPointer<PerpendicularMeasurementLineWidget>::New();
	m_lineWidget2->SetInteractor(this->Interactor);
	m_lineWidget2->CreateDefaultRepresentation();
	m_lineWidget2->GetLineRepresentation()->GetPoint1Representation()->SetPointPlacer(pointPlacer);
	m_lineWidget2->GetLineRepresentation()->GetPoint2Representation()->SetPointPlacer(pointPlacer);
	m_lineWidget2->GetLineRepresentation()->SetPoint1WorldPosition(
		m_centerLine->GetPoint(0));
	m_lineWidget2->GetLineRepresentation()->SetPoint2WorldPosition(
		m_centerLine->GetPoint(1));
	m_lineWidget2->SetLine(m_centerLine);
	m_lineWidget2->EnabledOn();

}

void InteractorStyleSurfaceCenterLinePerpendicularMeasurement::InitializeDistanceWidget()
{
	//m_distanceWidget = vtkSmartPointer<vtkDistanceWidget>::New();
	//m_distanceWidget->SetInteractor(this->Interactor);
	//m_distanceWidget->CreateDefaultRepresentation();
	//m_distanceWidget->ProcessEventsOff();
}

void InteractorStyleSurfaceCenterLinePerpendicularMeasurement::OnKeyPress()
{
	std::string key = this->Interactor->GetKeySym();
	cout << key << endl;
	if (key == "Return") {
		FindMaxiMumPerpendicularDistance();
	}
	else if (key == "Tab") {
		InteractorStyleSurfaceCenterLineSimpleClipping::OnKeyPress();
		InitializeLinePerpendicularMeasurementWidget();
	}
	else if (key == "space") {
		InteractorStyleSurfaceCenterLineSimpleClipping::OnKeyPress();
		InitializeLinePerpendicularMeasurementWidget();
	}
	else {
		InteractorStyleSurfaceCenterLineSimpleClipping::OnKeyPress();
	}
}


