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
//#include <vtkProperty.h>
#include <vtkSeedWidget.h>
#include <vtkSeedRepresentation.h>
#include <vtkPointHandleRepresentation3D.h>
//#include <vtkPolyDataPointPlacer.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkRenderer.h>
#include <vtkHandleWidget.h>




#include <vtkvmtkCapPolyData.h>
#include <vtkvmtkPolyDataCenterlines.h>

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


		vtkSmartPointer<vtkIdList> sourceIds = vtkSmartPointer<vtkIdList>::New();
		sourceIds->InsertNextId(capPolyData->GetCapCenterIds()->GetId(0));

		vtkSmartPointer<vtkIdList> targetIds = vtkSmartPointer<vtkIdList>::New();
		//targetIds->InsertNextId(capPolyData->GetCapCenterIds()->GetId(1));
		for (int i = 1; i < capPolyData->GetCapCenterIds()->GetNumberOfIds(); ++i) {
			targetIds->InsertNextId(capPolyData->GetCapCenterIds()->GetId(i));

		}

		vtkSmartPointer<vtkvmtkPolyDataCenterlines> centerlinesFilter = vtkSmartPointer<vtkvmtkPolyDataCenterlines>::New();
		centerlinesFilter->SetInputConnection(capPolyData->GetOutputPort());
		centerlinesFilter->SetSourceSeedIds(sourceIds);
		centerlinesFilter->SetTargetSeedIds(targetIds);
		centerlinesFilter->AppendEndPointsToCenterlinesOn();
		centerlinesFilter->SetRadiusArrayName("Radius");
		centerlinesFilter->SetEdgeArrayName("Edge");
		centerlinesFilter->SetEdgePCoordArrayName("PCoord");
		centerlinesFilter->Update();

		vtkSmartPointer<vtkPolyDataMapper> centerLineMapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		centerLineMapper->SetInputConnection(centerlinesFilter->GetOutputPort());
		centerLineMapper->Update();

		if (!m_centerLineActor) {
			m_centerLineActor =
				vtkSmartPointer<vtkActor>::New();
		}
		m_centerLineActor->SetMapper(centerLineMapper);

		vtkSmartPointer<vtkPolygonalSurfacePointPlacer> pointPlacer =
			vtkSmartPointer<vtkPolygonalSurfacePointPlacer>::New();
		pointPlacer->AddProp(m_centerLineActor);

		vtkSmartPointer<vtkPointHandleRepresentation3D> handleRep =
			vtkSmartPointer<vtkPointHandleRepresentation3D>::New();
		handleRep->SetPointPlacer(pointPlacer);

		if (!m_seedWidget) {
			m_seedWidget = vtkSmartPointer<vtkSeedWidget>::New();
		}
		m_seedWidget->CreateDefaultRepresentation();
		m_seedWidget->GetSeedRepresentation()->SetHandleRepresentation(handleRep);
		m_seedWidget->SetInteractor(this->Interactor);
		m_seedWidget->EnabledOn();
		m_seedWidget->CreateNewHandle()->EnabledOn();
		m_seedWidget->CreateNewHandle()->EnabledOn();
		m_seedWidget->CreateNewHandle()->EnabledOn();
		m_seedWidget->CompleteInteraction();

		m_surfaceViewer->GetRenderer()->AddActor(m_centerLineActor);
		m_surfaceViewer->Render();
	}
	else {
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

//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkImageViewer2.h>
//#include <vtkNIFTIImageReader.h>
//#include <vtkSmartPointer.h>
//#include <vtkPolyDataReader.h>
//#include <vtkInteractorStyle.h>


