#include "InteractorStyleSurfaceCenterLineSimpleClipping.h"

#include "SurfaceViewer.h"

#include <vtkObjectFactory.h>
#include <vtkClipPolyData.h>
#include <vtkBox.h>
#include <vtkImageData.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkThreshold.h>
#include <vtkDataSetAttributes.h>
#include <vtkGeometryFilter.h>


#include <vtkvmtkCapPolyData.h>
#include <vtkvmtkPolyDataCenterlines.h>

//#include <vtkPolyDataWriter.h>


vtkStandardNewMacro(InteractorStyleSurfaceCenterLineSimpleClipping);

void InteractorStyleSurfaceCenterLineSimpleClipping::SetCustomEnabled(bool flag)
{
	AbstractSurfaceCenterLine::SetCustomEnabled(flag);
}

InteractorStyleSurfaceCenterLineSimpleClipping::InteractorStyleSurfaceCenterLineSimpleClipping()
{
	m_ClipAndCapSurface = vtkSmartPointer<vtkPolyData>::New();
	m_capCenterIds = vtkSmartPointer<vtkIdList>::New();
}

InteractorStyleSurfaceCenterLineSimpleClipping::~InteractorStyleSurfaceCenterLineSimpleClipping()
{
}

void InteractorStyleSurfaceCenterLineSimpleClipping::CreateCenterLine()
{

	CreateCenterLine(true);
	//cout << "num of center id: " << capPolyData->GetCapCenterIds()->GetNumberOfIds() << endl;

}

void InteractorStyleSurfaceCenterLineSimpleClipping::CreateCenterLine(bool reClipSurface)
{
	if (!m_surfaceViewer->GetInput()) {
		vtkErrorMacro(<< "SurfaceViewer has no input");
		return;
	}
	if (reClipSurface) {
		m_sourceIdId = 0;
		ClipAndCap();
	}
	else {
		if (m_capCenterIds->GetNumberOfIds() <= 0) {
			vtkErrorMacro(<< "no cap center. ");
			vtkErrorMacro(<< "re cap anyway");
			m_sourceIdId = 0;
			ClipAndCap();
		}
		else if (m_sourceIdId < m_capCenterIds->GetNumberOfIds() - 1) {
			++m_sourceIdId;
		}
		else {
			m_sourceIdId = 0;
		}
	}
	if (m_capCenterIds->GetNumberOfIds() <= 0) {
		vtkErrorMacro(<< "still no cap center. ");
		return;
	}

	vtkSmartPointer<vtkIdList> sourceIds = nullptr;
	vtkSmartPointer<vtkIdList> targetIds = nullptr;

	sourceIds = vtkSmartPointer<vtkIdList>::New();
	sourceIds->InsertNextId(m_capCenterIds->GetId(m_sourceIdId));

	targetIds = vtkSmartPointer<vtkIdList>::New();
	for (int i = 0; i < m_capCenterIds->GetNumberOfIds(); ++i) {
		if (i != m_sourceIdId) {
			targetIds->InsertNextId(m_capCenterIds->GetId(i));
		}

	}

	vtkSmartPointer<vtkvmtkPolyDataCenterlines> centerlinesFilter = vtkSmartPointer<vtkvmtkPolyDataCenterlines>::New();
	centerlinesFilter->SetInputData(m_ClipAndCapSurface);
	centerlinesFilter->SetSourceSeedIds(sourceIds);
	centerlinesFilter->SetTargetSeedIds(targetIds);
	//centerlinesFilter->AppendEndPointsToCenterlinesOn();
	centerlinesFilter->SetRadiusArrayName("Radius");
	//centerlinesFilter->
	//centerlinesFilter->SetEdgeArrayName("Edge");
	//centerlinesFilter->SetEdgePCoordArrayName("PCoord");
	centerlinesFilter->Update();
	m_centerLine->ShallowCopy(centerlinesFilter->GetOutput());
}

void InteractorStyleSurfaceCenterLineSimpleClipping::ClipAndCap()
{
	double bounds[6];
	memcpy(bounds, m_surfaceViewer->GetSurfaceActor()->GetBounds(), sizeof(bounds));
	bounds[m_centerLineOrientation * 2] = bounds[m_centerLineOrientation * 2]
		+ GetSpacing()[m_centerLineOrientation] * m_ClipingDistance;
	bounds[m_centerLineOrientation * 2 + 1] = bounds[m_centerLineOrientation * 2 + 1]
		- GetSpacing()[m_centerLineOrientation] * m_ClipingDistance;
	vtkSmartPointer<vtkBox> clipBox =
		vtkSmartPointer<vtkBox>::New();
	clipBox->SetBounds(bounds);

	vtkSmartPointer<vtkThreshold> Threshold =
		vtkSmartPointer<vtkThreshold>::New();
	Threshold->SetInputData(m_surfaceViewer->GetSurfaceActor()->GetMapper()->GetInput());
	Threshold->ThresholdBetween(2,2);
	Threshold->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, vtkDataSetAttributes::SCALARS);
	Threshold->Update();

	vtkSmartPointer<vtkGeometryFilter> GeometryFilter =
		vtkSmartPointer<vtkGeometryFilter>::New();
	GeometryFilter->SetInputConnection(Threshold->GetOutputPort());
	GeometryFilter->Update();



	vtkSmartPointer<vtkClipPolyData> clipPolyData =
		vtkSmartPointer<vtkClipPolyData>::New();
	clipPolyData->SetInputConnection(GeometryFilter->GetOutputPort());
	//clipPolyData->SetInputData(m_surfaceViewer->GetSurfaceActor()->GetMapper()->GetInput());
	clipPolyData->SetClipFunction(clipBox);
	clipPolyData->InsideOutOn();
	clipPolyData->Update();
	//clipPolyData->GetOutput()->Print(cout);

	vtkSmartPointer<vtkvmtkCapPolyData> capPolyData =
		vtkSmartPointer<vtkvmtkCapPolyData>::New();
	capPolyData->SetInputConnection(clipPolyData->GetOutputPort());
	capPolyData->Update();

	m_ClipAndCapSurface->ShallowCopy(capPolyData->GetOutput());

	m_capCenterIds = capPolyData->GetCapCenterIds();

}

void InteractorStyleSurfaceCenterLineSimpleClipping::OnKeyPress()
{
	std::string key = this->Interactor->GetKeySym();
	cout << key << endl;
	if (key == "space") {
		CreateCenterLine(false);
		VisualizeCenterLine(m_centerLine);
	}
	else if (key == "Tab") {
		if (m_centerLineOrientation != CENTER_LINE_ORIENTATION_XY) {
			m_centerLineOrientation++;
		}
		else
		{
			m_centerLineOrientation = CENTER_LINE_ORIENTATION_YZ;
		}
		CreateCenterLine(true);
		VisualizeCenterLine(m_centerLine);
	}
	else {
		AbstractSurfaceCenterLine::OnKeyPress();
	}
}

