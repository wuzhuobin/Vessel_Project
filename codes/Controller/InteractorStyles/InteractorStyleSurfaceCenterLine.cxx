#include "InteractorStyleSurfaceCenterLine.h"

#include "SurfaceViewer.h"

#include <vtkObjectFactory.h>
#include <vtkClipPolyData.h>
#include <vtkBox.h>
#include <vtkImageData.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
//#include <vtkProperty.h>
#include <vtkSeedWidget.h>
#include <vtkSeedRepresentation.h>
#include <vtkPointHandleRepresentation3D.h>
//#include <vtkPolyDataPointPlacer.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkRenderer.h>
#include <vtkHandleWidget.h>
#include <vtkKdTreePointLocator.h>
#include <vtkDijkstraGraphGeodesicPath.h>
#include <vtkTriangleFilter.h>



#include <vtkvmtkCapPolyData.h>
#include <vtkvmtkPolyDataCenterlines.h>

//#include <vtkPolyDataWriter.h>


vtkStandardNewMacro(InteractorStyleSurfaceCenterLine);

void InteractorStyleSurfaceCenterLine::SetCustomEnabled(bool flag)
{
	AbstractInteractorStyleSurface::SetCustomEnabled(flag);
	

	if (m_customFlag) {

		double bounds[6];
		memcpy(bounds, m_surfaceViewer->GetSurfaceActor()->GetBounds(), sizeof(bounds));
		bounds[4] = bounds[4] + GetSpacing()[2] * 2;
		bounds[5] = bounds[5] - GetSpacing()[2] * 2;
		vtkSmartPointer<vtkBox> clipBox =
			vtkSmartPointer<vtkBox>::New();
		clipBox->SetBounds(bounds);

		vtkSmartPointer<vtkClipPolyData> clipPolyData =
			vtkSmartPointer<vtkClipPolyData>::New();
		clipPolyData->SetInputData(m_surfaceViewer->GetSurfaceActor()->GetMapper()->GetInput());
		clipPolyData->SetClipFunction(clipBox);
		clipPolyData->InsideOutOn();
		clipPolyData->Update();

		vtkSmartPointer<vtkvmtkCapPolyData> capPolyData =
			vtkSmartPointer<vtkvmtkCapPolyData>::New();
		capPolyData->SetInputConnection(clipPolyData->GetOutputPort());
		capPolyData->Update();
		capPolyData->Print(cout);
		cout << "num of center id: " << capPolyData->GetCapCenterIds()->GetNumberOfIds() << endl;


		m_sourceIds = vtkSmartPointer<vtkIdList>::New();
		m_sourceIds->InsertNextId(capPolyData->GetCapCenterIds()->GetId(0));

		m_targetIds = vtkSmartPointer<vtkIdList>::New();
		//m_targetIds->InsertNextId(capPolyData->GetCapCenterIds()->GetId(1));
		for (int i = 1; i < capPolyData->GetCapCenterIds()->GetNumberOfIds(); ++i) {
			m_targetIds->InsertNextId(capPolyData->GetCapCenterIds()->GetId(i));

		}

		vtkSmartPointer<vtkvmtkPolyDataCenterlines> centerlinesFilter = vtkSmartPointer<vtkvmtkPolyDataCenterlines>::New();
		centerlinesFilter->SetInputConnection(capPolyData->GetOutputPort());
		centerlinesFilter->SetSourceSeedIds(m_sourceIds);
		centerlinesFilter->SetTargetSeedIds(m_targetIds);
		centerlinesFilter->AppendEndPointsToCenterlinesOn();
		//centerlinesFilter->SetCapCenterIds(capPolyData->GetCapCenterIds());
		centerlinesFilter->SetRadiusArrayName("Radius");
		centerlinesFilter->SetEdgeArrayName("Edge");
		centerlinesFilter->SetEdgePCoordArrayName("PCoord");
		centerlinesFilter->Update();

		m_centerLine = centerlinesFilter->GetOutput();

		vtkSmartPointer<vtkPolyDataMapper> centerLineMapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		centerLineMapper->SetInputConnection(centerlinesFilter->GetOutputPort());
		centerLineMapper->Update();

		//if (!m_centerLineActor) {
			m_centerLineActor =
				vtkSmartPointer<vtkActor>::New();
		//}
		m_centerLineActor->SetMapper(centerLineMapper);

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
			capPolyData->GetOutput()->GetPoint(
				capPolyData->GetCapCenterIds()->GetId(0));

			handle->GetHandleRepresentation()->SetWorldPosition(worldPos);
			handle->EnabledOn();
		}


		m_surfaceViewer->GetRenderer()->AddActor(m_centerLineActor);
		m_surfaceViewer->Render();



		m_pointLocator =
			vtkSmartPointer<vtkKdTreePointLocator>::New();
		m_pointLocator->SetDataSet(centerlinesFilter->GetOutput());
		m_pointLocator->BuildLocator();
		//vtkSmartPointer<vtkPolyDataWriter> writer =
		//	vtkSmartPointer<vtkPolyDataWriter>::New();
		//writer->SetInputData(centerlinesFilter->GetOutput());
		//writer->SetFileName("C:/Users/user/Desktop/centerline.vtk");
		//writer->Write();

	}
	else {
		if (m_pointLocator) {
			m_pointLocator = nullptr;
		}
		if (m_centerLine) {
			m_centerLine = nullptr;
		}
		if (m_centerLineActor) {
			m_surfaceViewer->GetRenderer()->RemoveActor(m_centerLineActor);
			m_centerLineActor = nullptr;
		}
		if (m_seedWidget) {
			m_seedWidget->EnabledOff();
			m_seedWidget = nullptr;
		}
		m_surfaceViewer->Render();
	}
}

void InteractorStyleSurfaceCenterLine::OnKeyPress()
{
	std::string key = this->Interactor->GetKeySym();
	cout << key << endl;
	if (key == "space") {

		double worldPos[3];
		m_seedWidget->GetSeedRepresentation()->GetSeedWorldPosition(0, worldPos);
		vtkIdType seed1 = m_pointLocator->FindClosestPoint(worldPos);
		m_seedWidget->GetSeedRepresentation()->GetSeedWorldPosition(1, worldPos);
		vtkIdType seed2 = m_pointLocator->FindClosestPoint(worldPos);


		vtkSmartPointer<vtkTriangleFilter> triangleFilter =
			vtkSmartPointer<vtkTriangleFilter>::New();
		triangleFilter->SetInputData(m_centerLine);
		triangleFilter->Update();

		vtkSmartPointer<vtkDijkstraGraphGeodesicPath> dijkstra =
			vtkSmartPointer<vtkDijkstraGraphGeodesicPath>::New();
		dijkstra->SetInputData(triangleFilter->GetOutput());
		//dijkstra->SetInputData(m_centerLine);
		dijkstra->SetStartVertex(seed1);
		dijkstra->SetEndVertex(seed2);
		dijkstra->Update();
		vtkIdList* pathPointId = dijkstra->GetIdList();
		for (vtkIdType id = 0; id < pathPointId->GetNumberOfIds(); ++id) {
			cout << "point id = " << id << endl;
			double* point = m_centerLine->GetPoint(pathPointId->GetId(id));
			cout << point[0] << ' ' << point[1] << ' ' << point[2] << endl;
		}

	}
	else {
		AbstractInteractorStyleSurface::OnKeyPress();
	}
}


//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkImageViewer2.h>
//#include <vtkNIFTIImageReader.h>
//#include <vtkSmartPointer.h>
//#include <vtkPolyDataReader.h>
//#include <vtkInteractorStyle.h>


