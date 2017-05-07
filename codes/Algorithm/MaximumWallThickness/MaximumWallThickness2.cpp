#include "MaximumWallThickness2.h"

#include <vtkObjectFactory.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkMarchingSquares.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkCenterOfMass.h>

vtkStandardNewMacro(MaximumWallThickness2);

void MaximumWallThickness2::PrintSelf(ostream & os, vtkIndent indent)
{
	vtkThreadedImageAlgorithm::PrintSelf(os, indent);
}

MaximumWallThickness2::MaximumWallThickness2()
{
}

MaximumWallThickness2::~MaximumWallThickness2()
{
}

void MaximumWallThickness2::ThreadedRequestData(vtkInformation * request, vtkInformationVector ** inputVector, vtkInformationVector * outputVector, vtkImageData *** inData, vtkImageData ** outData, int extent[6], int id)
{
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	
	for (int range = extent[4]; range <= extent[5]; ++range) {

		int _extent[6];
		memcpy(_extent, extent, sizeof(_extent));
		_extent[4] = _extent[5] = range;
		vtkSmartPointer<vtkMarchingSquares> lumenContours =
			vtkSmartPointer<vtkMarchingSquares>::New();
		lumenContours->SetInputData(inData[0][0]);
		lumenContours->SetImageRange(_extent);
		lumenContours->GenerateValues(1, LumenIntensity, LumenIntensity);
		lumenContours->Update();

		vtkSmartPointer<vtkPolyDataConnectivityFilter> lumenConnectivity =
			vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
		lumenConnectivity->SetInputConnection(lumenContours->GetOutputPort());
		lumenConnectivity->SetExtractionModeToAllRegions();
		lumenConnectivity->Update();
		int numOfLumens = lumenConnectivity->GetNumberOfExtractedRegions();

		for (int i = 0; i < numOfLumens; i++)
		{
			vtkSmartPointer<vtkPolyDataConnectivityFilter> _lumenConnectivity =
				vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
			_lumenConnectivity->SetInputConnection(lumenContours->GetOutputPort());
			_lumenConnectivity->AddSpecifiedRegion(i);
			_lumenConnectivity->SetExtractionModeToSpecifiedRegions();
			_lumenConnectivity->Update();

			vtkSmartPointer<vtkPolyDataConnectivityFilter> vesselWallContours =
				vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
			vesselWallContours->SetInputConnection(lumenContours->GetOutputPort());
			vesselWallContours->AddSpecifiedRegion(i);
			vesselWallContours->SetExtractionModeToAllRegions();
			vesselWallContours->Update();

			int numOfVesselWalls = lumenConnectivity->GetNumberOfExtractedRegions();


			for (int j = 0; j < numOfVesselWalls; j++)
			{

			}

		}


		vtkSmartPointer<vtkMarchingSquares> vesselWallContours =
			vtkSmartPointer<vtkMarchingSquares>::New();
		vesselWallContours->SetInputData(inData[0][0]);
		vesselWallContours->SetImageRange(_extent);
		vesselWallContours->GenerateValues(1, VesselWallIntensity, VesselWallIntensity);
		vesselWallContours->Update();

	}

}
