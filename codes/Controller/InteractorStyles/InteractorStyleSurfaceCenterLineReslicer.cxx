#include "InteractorStyleSurfaceCenterLineReslicer.h"

#include "SurfaceViewer.h"

#include <vtkObjectFactory.h>
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
//#include <vtkTextActor.h>
//#include <vtkTextProperty.h>
#include <vtkRenderer.h>
#include <vtkCommand.h>
#include <vtkImageActor.h>
#include <vtkImageMapper3D.h>
#include <vtkImageProperty.h>
#include <vtkLookupTable.h>
#include <vtkvmtkCurvedMPRImageFilter.h>
#include <vtkvmtkCenterlineGeometry.h>
#include <vtkvmtkCenterlineAttributesFilter.h>

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

class InteractorStyleSurfaceCenterLineReslicerCallback : public vtkCommand {
public:
	static InteractorStyleSurfaceCenterLineReslicerCallback* New() { return new InteractorStyleSurfaceCenterLineReslicerCallback; }
	InteractorStyleSurfaceCenterLineReslicer* style;
	virtual void Execute(vtkObject*, unsigned long event, void *calldata) override {
		if (event == vtkCommand::EndInteractionEvent)
		{
			style->UpdateReslicer();
		}
	}
};

vtkStandardNewMacro(InteractorStyleSurfaceCenterLineReslicer);

void InteractorStyleSurfaceCenterLineReslicer::SetCustomEnabled(bool flag)
{
	InteractorStyleSurfaceCenterLineSimpleClipping::SetCustomEnabled(flag);
	if (m_customFlag) {
		InitializeHandleWidgets();
		//m_surfaceViewer->GetRenderer()->AddActor(m_measurementText);
		InitializeReslicer();
	}
	else {
		for (int i = 0; i < NUM_OF_HANDLES; ++i) {
			// removing widgets
			m_handleWidgets[i]->SetInteractor(nullptr);
			m_handleWidgets[i]->EnabledOff();
			m_handleWidgets[i]->RemoveAllObservers();
			m_handleWidgets[i] = nullptr;
		}
		m_pointLocator = nullptr;
		//m_surfaceViewer->GetRenderer()->RemoveActor(m_measurementText);
		m_imageActor = nullptr;
	}
	

}

InteractorStyleSurfaceCenterLineReslicer::InteractorStyleSurfaceCenterLineReslicer()
{
	//m_measurementText =
	//	vtkSmartPointer<vtkTextActor>::New();
	//m_measurementText->SetPosition2(10, 40);
	//m_measurementText->GetTextProperty()->SetFontSize(24);
	//m_measurementText->GetTextProperty()->SetColor(1.0, 0.0, 0.0);
}

InteractorStyleSurfaceCenterLineReslicer::~InteractorStyleSurfaceCenterLineReslicer()
{
}

//void InteractorStyleSurfaceCenterLineReslicer::CreateCenterLine()
//{
//	InteractorStyleSurfaceCenterLineSimpleClipping::CreateCenterLine();
//}
#include <vtkPolyDataWriter.h>
void InteractorStyleSurfaceCenterLineReslicer::InitializeHandleWidgets()
{
	//vtkSmartPointer<vtkCleanPolyData> cleanPolyData =
	//	vtkSmartPointer<vtkCleanPolyData>::New();
	//cleanPolyData->SetInputData(m_centerLine);
	//cleanPolyData->PointMergingOn();
	////cleanPolyData->PointMergingOff();
	//cleanPolyData->Update();

	//vtkSmartPointer<vtkTriangleFilter> triangleFilter =
	//	vtkSmartPointer<vtkTriangleFilter>::New();
	//triangleFilter->SetInputConnection(cleanPolyData->GetOutputPort());
	//triangleFilter->Update();



	m_pointLocator = vtkSmartPointer<vtkKdTreePointLocator>::New();
	m_pointLocator->SetDataSet(m_centerLine);
	m_pointLocator->BuildLocator();

	vtkSmartPointer<vtkPolygonalSurfacePointPlacer> pointPlacer =
		vtkSmartPointer<vtkPolygonalSurfacePointPlacer>::New();
	pointPlacer->AddProp(m_centerLineActor);

	double* worldPos =
		m_centerLine->GetPoint(0);

	for (int i = 0; i < NUM_OF_HANDLES; ++i) {

		// the vtkSphereHandleRepresentation does not use the vtkPolygonalSurfacePointPlacer 
		// in its widgetInteraction function, but vtkPointHandleRepresentation3D does
		vtkSmartPointer<vtkPointHandleRepresentation3D> handleRep =
			vtkSmartPointer<vtkPointHandleRepresentation3D>::New();
		handleRep->SetPointPlacer(pointPlacer);
		handleRep->SetWorldPosition(m_centerLine->GetPoint(i));

		vtkSmartPointer<InteractorStyleSurfaceCenterLineReslicerCallback> callback =
			vtkSmartPointer<InteractorStyleSurfaceCenterLineReslicerCallback>::New();
		callback->style = this;

		m_handleWidgets[i] = vtkSmartPointer<vtkHandleWidget>::New();
		m_handleWidgets[i]->SetRepresentation(handleRep);
		m_handleWidgets[i]->SetInteractor(this->Interactor);
		m_handleWidgets[i]->EnabledOn();
		m_handleWidgets[i]->AddObserver(vtkCommand::EndInteractionEvent, callback);

	}



	//vtkSmartPointer<vtkPolyDataWriter> writer =
	//	vtkSmartPointer<vtkPolyDataWriter>::New();
	//writer->SetInputData(centerlinesFilter->GetOutput());
	//writer->SetFileName("C:/Users/user/Desktop/centerline.vtk");
	//writer->Write();
}
#include <vtkNIFTIImageWriter.h>
void InteractorStyleSurfaceCenterLineReslicer::InitializeReslicer()
{
	vtkSmartPointer<vtkvmtkCenterlineGeometry> centerlineGeometry =
		vtkSmartPointer<vtkvmtkCenterlineGeometry>::New();
	centerlineGeometry->SetInputData(m_centerLine);
	centerlineGeometry->SetLengthArrayName("Length");
	centerlineGeometry->SetCurvatureArrayName("Curvature");
	centerlineGeometry->SetTorsionArrayName("Torsion");
	centerlineGeometry->SetTortuosityArrayName("Tortuosity");
	centerlineGeometry->SetFrenetTangentArrayName("FrenetTangent");
	centerlineGeometry->SetFrenetNormalArrayName("FrenetNormal");
	centerlineGeometry->SetFrenetBinormalArrayName("FrenetBinormal");
	centerlineGeometry->LineSmoothingOff();
	centerlineGeometry->Update();

	vtkSmartPointer<vtkvmtkCenterlineAttributesFilter> centerlineAttributes =
		vtkSmartPointer<vtkvmtkCenterlineAttributesFilter>::New();
	centerlineAttributes->SetInputConnection(centerlineGeometry->GetOutputPort());
	centerlineAttributes->SetAbscissasArrayName("Abscissas");
	centerlineAttributes->SetParallelTransportNormalsArrayName("Normals");
	centerlineAttributes->Update();



	vtkSmartPointer<vtkvmtkCurvedMPRImageFilter> curvedMPRImageFilter =
		vtkSmartPointer<vtkvmtkCurvedMPRImageFilter>::New();
	curvedMPRImageFilter->SetInputData(m_surfaceViewer->GetInput());
	curvedMPRImageFilter->SetCenterline(centerlineAttributes->GetOutput());
	//centerlineAttributesFilter->SetInputConnection(triangleFilter->GetOutputPort());
	curvedMPRImageFilter->SetParallelTransportNormalsArrayName("Normals");
	curvedMPRImageFilter->SetFrenetTangentArrayName("FrenetTangent");
	curvedMPRImageFilter->SetInplaneOutputSpacing(GetSpacing()[0], GetSpacing()[1]);
	curvedMPRImageFilter->SetInplaneOutputSize(GetExtent()[1] - GetExtent()[0], GetExtent()[3] - GetExtent()[2]);
	curvedMPRImageFilter->SetReslicingBackgroundLevel(0.0);
	curvedMPRImageFilter->Update();
	cout << GetOrigin()[0] << endl;
	cout << GetOrigin()[1] << endl;
	cout << GetOrigin()[2] << endl;
	cout << curvedMPRImageFilter->GetOutputOrigin()[0] << endl;
	cout << curvedMPRImageFilter->GetOutputOrigin()[1] << endl;
	cout << curvedMPRImageFilter->GetOutputOrigin()[2] << endl;
	//m_centerLine = curvedMPRImageFilter->GetCenterline();

	//vtkSmartPointer<vtkNIFTIImageWriter> nIFTIImageWriter =
	//	vtkSmartPointer<vtkNIFTIImageWriter>::New();
	//nIFTIImageWriter->SetInputData(curvedMPRImageFilter->GetOutput());
	//nIFTIImageWriter->SetFileName("C:/Users/jieji/Desktop/curved.nii");
	//nIFTIImageWriter->Update();

	//vtkSmartPointer<vtkPolyDataWriter> w =
	//	vtkSmartPointer<vtkPolyDataWriter>::New();
	//w->SetInputData(centerlineAttributes->GetOutput());
	//w->SetFileName("C:/Users/jieji/Desktop/centerline.vtk");
	//w->Update();

	m_imageActor = vtkSmartPointer<vtkImageActor>::New();
	m_imageActor->GetMapper()->SetInputConnection(curvedMPRImageFilter->GetOutputPort());
	m_imageActor->GetProperty()->SetLookupTable(m_surfaceViewer->GetLookupTable());
	m_imageActor->GetProperty()->UseLookupTableScalarRangeOn();
	m_surfaceViewer->GetRenderer()->AddActor(m_imageActor);
}

void InteractorStyleSurfaceCenterLineReslicer::UpdateReslicer()
{
	const double* worldPos = m_handleWidgets[0]->GetHandleRepresentation()->GetWorldPosition();

	
	//int k = (worldPos[2] - GetOrigin()[2]) / GetSpacing()[2] + 0.5;
	int k = m_pointLocator->FindClosestPoint(worldPos);

	m_imageActor->SetDisplayExtent(
		GetExtent()[0], 
		GetExtent()[1], 
		GetExtent()[2], 
		GetExtent()[3], 
		k,
		k);
	m_imageActor->Render(m_surfaceViewer->GetRenderer());
}

//void InteractorStyleSurfaceCenterLineReslicer::FindMaximumRadius()
//{
//	vtkIdType seed1 = m_pointLocator->FindClosestPoint(
//		m_handleWidgets[0]->GetHandleRepresentation()->GetWorldPosition());
//	vtkIdType seed2 = m_pointLocator->FindClosestPoint(
//		m_handleWidgets[1]->GetHandleRepresentation()->GetWorldPosition());
//	vtkSmartPointer<vtkDijkstraGraphGeodesicPathDistance> dijkstra =
//		vtkSmartPointer<vtkDijkstraGraphGeodesicPathDistance>::New();
//	dijkstra->SetInputData(m_triangulatedCenterLine);
//	dijkstra->SetStartVertex(seed1);
//	//dijkstra->SetRepelVertices(repelVertices);
//	//dijkstra->RepelPathFromVerticesOn();
//	dijkstra->SetEndVertex(seed2);
//	dijkstra->StopWhenEndReachedOn();
//	dijkstra->Update();
//	vtkIdList* pathPointId = dijkstra->GetIdList();
//	//vtkSmartPointer<vtkDoubleArray> radius =
//	//	vtkSmartPointer<vtkDoubleArray>::New();
//	//radius->SetNumberOfValues(pathPointId->GetNumberOfIds());
//	double maxRadius = VTK_DOUBLE_MIN;
//	double minRadius = VTK_DOUBLE_MAX;
//	double GeodesicPathDistance = 0;
//	vtkIdType maxRadiusId = 0;
//	vtkIdType minRadiusId = 0;
//	for (vtkIdType id = 0; id < pathPointId->GetNumberOfIds(); ++id) {
//		vtkIdType _pointId = pathPointId->GetId(id);
//		double* value = static_cast<double*>(
//			m_triangulatedCenterLine->GetPointData()->GetArray("Radius")->GetVoidPointer(_pointId));
//		if (*value > maxRadius) {
//			maxRadius = *value;
//			maxRadiusId = _pointId;
//		}
//		if (*value < minRadius) {
//			minRadius = *value;
//			minRadiusId = _pointId;
//		}
//		//cout << "point id = " << id << endl;
//		//double* point = m_centerLine->GetPoint(pathPointId->GetId(id));
//		//cout << point[0] << ' ' << point[1] << ' ' << point[2] << endl;
//	}
//	cout << "maximum radius " << maxRadiusId << " is " << maxRadius << endl;
//	cout << "minimum radius " << minRadiusId << " is " << minRadius << endl;
//	GeodesicPathDistance = dijkstra->GetGeodesicPathDistance(seed2);
//	cout << GeodesicPathDistance << endl;
//	char buff[100];
//	sprintf(buff, "Maximum radius: %.2f mm\n Minimum radius: %.2f mm\n Center line length: %.2f mm", maxRadius, minRadius, GeodesicPathDistance);
//	m_measurementText->SetInput(buff);
//	this->m_surfaceViewer->Render();
//}

void InteractorStyleSurfaceCenterLineReslicer::OnKeyPress()
{
	std::string key = this->Interactor->GetKeySym();
	cout << key << endl;
	if (key == "Tab") {
		InteractorStyleSurfaceCenterLineSimpleClipping::OnKeyPress();
		InitializeHandleWidgets();
	}
	else if (key == "space") {
		InteractorStyleSurfaceCenterLineSimpleClipping::OnKeyPress();
		InitializeHandleWidgets();
	}
	else {
		InteractorStyleSurfaceCenterLineSimpleClipping::OnKeyPress();
	}
}


