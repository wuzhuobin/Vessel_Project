#include "PerpendicularMeasurementLineWidget.h"

#include <vtkObjectFactory.h>
#include <vtkWidgetCallbackMapper.h>
#include <vtkCommand.h>
#include <vtkWidgetEvent.h>
#include <vtkHandleWidget.h>
#include <vtkRenderWindow.h>
#include <vtkCallbackCommand.h>
#include <vtkDistanceWidget.h>
#include <vtkLineRepresentation.h>
#include <vtkPolyData.h>
#include <vtkLine.h>
#include <vtkDistanceRepresentation2D.h>
#include <vtkCleanPolyData.h>
#include <vtkTriangleFilter.h>
#include <vtkDijkstraGraphGeodesicPath.h>
#include <vtkKdTreePointLocator.h>

vtkStandardNewMacro(PerpendicularMeasurementLineWidget);

void PerpendicularMeasurementLineWidget::PrintSelf(ostream & os, vtkIndent indent)
{
	vtkLineWidget2::PrintSelf(os, indent);
	this->DistanceWidget->PrintSelf(os, indent);
}

void PerpendicularMeasurementLineWidget::SetEnabled(int enabling)
{
	vtkLineWidget2::SetEnabled(enabling);
	if (enabling) {
		this->DistanceWidget->SetWidgetStateToManipulate();
		this->DistanceWidget->SetInteractor(this->Interactor);
	}
	//this->DistanceWidget->SetEnabled(enabling);
}

void PerpendicularMeasurementLineWidget::CreateDefaultRepresentation()
{
	vtkLineWidget2::CreateDefaultRepresentation();
	this->DistanceWidget->CreateDefaultRepresentation();
}

void PerpendicularMeasurementLineWidget::SetLine(vtkPolyData * line)
{
	vtkSmartPointer<vtkCleanPolyData> cleanPolyData =
		vtkSmartPointer<vtkCleanPolyData>::New();
	cleanPolyData->SetInputData(line);
	cleanPolyData->PointMergingOn();
	//cleanPolyData->PointMergingOff();
	cleanPolyData->Update();

	vtkSmartPointer<vtkTriangleFilter> triangleFilter =
		vtkSmartPointer<vtkTriangleFilter>::New();
	triangleFilter->SetInputConnection(cleanPolyData->GetOutputPort());
	triangleFilter->Update();
	TriangulatedLine = triangleFilter->GetOutput();

	PointLocater = vtkSmartPointer<vtkKdTreePointLocator>::New();
	PointLocater->SetDataSet(TriangulatedLine);
	PointLocater->BuildLocator();
}

vtkDistanceWidget * PerpendicularMeasurementLineWidget::GetDistanceWidget()
{
	return this->DistanceWidget;
}

void PerpendicularMeasurementLineWidget::SetProcessEvents(int flag)
{
	vtkAbstractWidget::SetProcessEvents(flag);

	this->Point1Widget->SetProcessEvents(flag);
	this->Point2Widget->SetProcessEvents(flag);
}

PerpendicularMeasurementLineWidget::PerpendicularMeasurementLineWidget()
{
	// Define widget events
	// remove unnecessary event by a do nothing callback lambda function
	this->CallbackMapper->SetCallbackMethod(vtkCommand::LeftButtonReleaseEvent,
		vtkWidgetEvent::EndSelect,
		this, PerpendicularMeasurementLineWidget::EndSelectAction);
	this->CallbackMapper->SetCallbackMethod(vtkCommand::MiddleButtonPressEvent,
		vtkWidgetEvent::Translate,
		this, [](vtkAbstractWidget*) {});
	this->CallbackMapper->SetCallbackMethod(vtkCommand::MiddleButtonReleaseEvent,
		vtkWidgetEvent::EndTranslate,
		this, [](vtkAbstractWidget*) {});
	this->CallbackMapper->SetCallbackMethod(vtkCommand::RightButtonPressEvent,
		vtkWidgetEvent::Scale,
		this, [](vtkAbstractWidget*) {});
	this->CallbackMapper->SetCallbackMethod(vtkCommand::RightButtonReleaseEvent,
		vtkWidgetEvent::EndScale,
		this, [](vtkAbstractWidget*) {});

	// disable the #LineHandle widget reponds to interaction
	this->LineHandle->ProcessEventsOff();

	DistanceWidget = vtkDistanceWidget::New();
	DistanceWidget->ProcessEventsOff();
}

PerpendicularMeasurementLineWidget::~PerpendicularMeasurementLineWidget()
{
	DistanceWidget->Delete();
}

void PerpendicularMeasurementLineWidget::EndSelectAction(vtkAbstractWidget * w)
{
	PerpendicularMeasurementLineWidget *self =
		reinterpret_cast<PerpendicularMeasurementLineWidget*>(w);
	if (self->WidgetState == vtkLineWidget2::Active && self->TriangulatedLine != nullptr)
	{
		double pos1[3], pos2[3];
		self->GetLineRepresentation()->GetPoint1WorldPosition(pos1);
		self->GetLineRepresentation()->GetPoint2WorldPosition(pos2);

		vtkSmartPointer<vtkDijkstraGraphGeodesicPath> geodesicPath =
			vtkSmartPointer<vtkDijkstraGraphGeodesicPath>::New();
		geodesicPath->SetInputData(self->TriangulatedLine);
		geodesicPath->SetStartVertex(self->PointLocater->FindClosestPoint(pos1));
		geodesicPath->SetEndVertex(self->PointLocater->FindClosestPoint(pos2));
		geodesicPath->Update();

		double perpendicularDistance = VTK_DOUBLE_MIN;
		double perpendicularPoint1[3], perpendicularPoint2[3];
		for (vtkIdType id = 0; id < geodesicPath->GetOutput()->GetNumberOfPoints(); ++id) {
			double t;
			double closest[3];
			double dist0 = vtkLine::DistanceToLine(geodesicPath->GetOutput()->GetPoint(id), pos1, pos2, t, closest);
			if (dist0 > perpendicularDistance) {
				perpendicularDistance = dist0;
				memcpy(perpendicularPoint1, geodesicPath->GetOutput()->GetPoint(id), sizeof(perpendicularPoint1));
				memcpy(perpendicularPoint2, closest, sizeof(perpendicularPoint2));
			}
		}
		cout << "farthest distance: " << sqrt(perpendicularDistance)<< endl;
		self->DistanceWidget->EnabledOn();
		self->DistanceWidget->GetDistanceRepresentation()->SetPoint1WorldPosition(perpendicularPoint1);
		self->DistanceWidget->GetDistanceRepresentation()->SetPoint2WorldPosition(perpendicularPoint2);
	}
	vtkLineWidget2::EndSelectAction(w);

}
