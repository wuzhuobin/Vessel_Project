#include "InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius.h"

#include "SurfaceViewer.h"

#include <vtkObjectFactory.h>
#include <vtkSeedWidget.h>
#include <vtkSeedRepresentation.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkCleanPolyData.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkTriangleFilter.h>
#include <vtkActor.h>
#include <vtkHandleWidget.h>
#include <vtkKdTreePointLocator.h>
#include <vtkDijkstraGraphGeodesicPath.h>
//#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkIdList.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkRenderer.h>

#include <vtkDijkstraGraphInternals.h>
class vtkDijkstraGraphGeodesicPathDistance : public vtkDijkstraGraphGeodesicPath {

public:
	static vtkDijkstraGraphGeodesicPathDistance* New() { return new vtkDijkstraGraphGeodesicPathDistance(); }
	double GetGeodesicPathDistance(int vertexId) {
		try
		{
			return this->Internals->CumulativeWeights.at(vertexId);
		}
		catch (const std::out_of_range& e)
		{
			cerr << e.what();
			vtkErrorMacro(<< e.what());
			return VTK_DOUBLE_MAX;
		}

	}
	//vtkGetMacro(GeodesicPathDistance, double);
protected:
	double GeodesicPathDistance = 0;

};

vtkStandardNewMacro(InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius);

void InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::SetCustomEnabled(bool flag)
{
	InteractorStyleSurfaceCenterLineSimpleClipping::SetCustomEnabled(flag);
	if (m_customFlag) {
		InitializationSeeds();
		m_surfaceViewer->GetRenderer()->AddActor(m_radiusText);
	}
	else {
		m_seedWidget->EnabledOff();
		m_seedWidget = nullptr;
		m_pointLocator = nullptr;
		m_surfaceViewer->GetRenderer()->RemoveActor(m_radiusText);
	}
	

}

InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius()
{
	m_radiusText =
		vtkSmartPointer<vtkTextActor>::New();
	m_radiusText->SetPosition2(10, 40);
	m_radiusText->GetTextProperty()->SetFontSize(24);
	m_radiusText->GetTextProperty()->SetColor(1.0, 0.0, 0.0);
}

InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::~InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius()
{
}

void InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::CreateCenterLine()
{
	InteractorStyleSurfaceCenterLineSimpleClipping::CreateCenterLine();
}

void InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::InitializationSeeds()
{
	vtkSmartPointer<vtkCleanPolyData> cleanPolyData =
		vtkSmartPointer<vtkCleanPolyData>::New();
	cleanPolyData->SetInputData(m_centerLine);
	cleanPolyData->PointMergingOn();
	//cleanPolyData->PointMergingOff();
	cleanPolyData->Update();

	vtkSmartPointer<vtkTriangleFilter> triangleFilter =
		vtkSmartPointer<vtkTriangleFilter>::New();
	triangleFilter->SetInputConnection(cleanPolyData->GetOutputPort());
	triangleFilter->Update();
	m_triangulatedCenterLine = triangleFilter->GetOutput();

	vtkSmartPointer<vtkPolygonalSurfacePointPlacer> pointPlacer =
		vtkSmartPointer<vtkPolygonalSurfacePointPlacer>::New();
	pointPlacer->AddProp(m_centerLineActor);

	vtkSmartPointer<vtkPointHandleRepresentation3D> handleRep =
		vtkSmartPointer<vtkPointHandleRepresentation3D>::New();
	handleRep->SetPointPlacer(pointPlacer);

	//if (!m_seedWidget) {
	m_seedWidget = vtkSmartPointer<vtkSeedWidget>::New();
	//}
	m_seedWidget->CreateDefaultRepresentation();
	m_seedWidget->GetSeedRepresentation()->SetHandleRepresentation(handleRep);
	m_seedWidget->SetInteractor(this->Interactor);
	m_seedWidget->EnabledOn();
	m_seedWidget->CompleteInteraction();
	// testing handlewidget
	
	for (int i = 0; i < NUM_OF_SEEDS; ++i) {
		vtkHandleWidget* handle = m_seedWidget->CreateNewHandle();
		double* worldPos =
			m_triangulatedCenterLine->GetPoint(0);

		handle->GetHandleRepresentation()->SetWorldPosition(worldPos);
		handle->EnabledOn();
	}

	m_pointLocator = vtkSmartPointer<vtkKdTreePointLocator>::New();
	m_pointLocator->SetDataSet(m_triangulatedCenterLine);
	m_pointLocator->BuildLocator();
	//vtkSmartPointer<vtkPolyDataWriter> writer =
	//	vtkSmartPointer<vtkPolyDataWriter>::New();
	//writer->SetInputData(centerlinesFilter->GetOutput());
	//writer->SetFileName("C:/Users/user/Desktop/centerline.vtk");
	//writer->Write();
}

void InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::FindMaximumRadius()
{
	double worldPos[3];
	m_seedWidget->GetSeedRepresentation()->GetSeedWorldPosition(0, worldPos);
	vtkIdType seed1 = m_pointLocator->FindClosestPoint(worldPos);
	m_seedWidget->GetSeedRepresentation()->GetSeedWorldPosition(1, worldPos);
	vtkIdType seed2 = m_pointLocator->FindClosestPoint(worldPos);
	vtkSmartPointer<vtkDijkstraGraphGeodesicPathDistance> dijkstra =
		vtkSmartPointer<vtkDijkstraGraphGeodesicPathDistance>::New();
	dijkstra->SetInputData(m_triangulatedCenterLine);
	dijkstra->SetStartVertex(seed1);
	//dijkstra->SetRepelVertices(repelVertices);
	//dijkstra->RepelPathFromVerticesOn();
	dijkstra->SetEndVertex(seed2);
	dijkstra->StopWhenEndReachedOn();
	dijkstra->Update();
	vtkIdList* pathPointId = dijkstra->GetIdList();
	//vtkSmartPointer<vtkDoubleArray> radius =
	//	vtkSmartPointer<vtkDoubleArray>::New();
	//radius->SetNumberOfValues(pathPointId->GetNumberOfIds());
	double maxRadius = VTK_DOUBLE_MIN;
	double minRadius = VTK_DOUBLE_MAX;
	double GeodesicPathDistance = 0;
	vtkIdType maxRadiusId = 0;
	vtkIdType minRadiusId = 0;
	for (vtkIdType id = 0; id < pathPointId->GetNumberOfIds(); ++id) {
		vtkIdType _pointId = pathPointId->GetId(id);
		double* value = static_cast<double*>(
			m_triangulatedCenterLine->GetPointData()->GetArray("Radius")->GetVoidPointer(_pointId));
		if (*value > maxRadius) {
			maxRadius = *value;
			maxRadiusId = _pointId;
		}
		if (*value < minRadius) {
			minRadius = *value;
			minRadiusId = _pointId;
		}
		//cout << "point id = " << id << endl;
		//double* point = m_centerLine->GetPoint(pathPointId->GetId(id));
		//cout << point[0] << ' ' << point[1] << ' ' << point[2] << endl;
	}
	cout << "maximum radius " << maxRadiusId << " is " << maxRadius << endl;
	cout << "minimum radius " << minRadiusId << " is " << minRadius << endl;
	GeodesicPathDistance = dijkstra->GetGeodesicPathDistance(seed2);
	cout << GeodesicPathDistance << endl;
	char buff[100];
	sprintf(buff, "Maximum radius: %.2f mm\n Minimum radius: %.2f mm\n Center line length: %.2f mm", maxRadius, minRadius, GeodesicPathDistance);
	m_radiusText->SetInput(buff);
}

void InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::OnKeyPress()
{
	std::string key = this->Interactor->GetKeySym();
	cout << key << endl;
	if (key == "Return") {
		FindMaximumRadius();
	}
	else if (key == "Tab") {
		InteractorStyleSurfaceCenterLineSimpleClipping::OnKeyPress();
		InitializationSeeds();
	}
	else if (key == "space") {
		InteractorStyleSurfaceCenterLineSimpleClipping::OnKeyPress();
		InitializationSeeds();
	}
	else {
		InteractorStyleSurfaceCenterLineSimpleClipping::OnKeyPress();
	}
}


