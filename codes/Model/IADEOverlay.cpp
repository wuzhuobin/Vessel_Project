#include "IADEOverlay.h"

#include <vtkLookupTable.h>

#include <vtkImageAccumulate.h>
#include <vtkExtractVOI.h>
IADEOverlay::IADEOverlay(QObject * parent)
	:Overlay(parent)
{
	// using m_overlayColor to build the lookupTable
	m_overlayColor[0] = zeros;
	m_overlayColor[1] = vesselWall;
	m_overlayColor[2] = lumen;
	m_overlayColor[3] = calcification;
	m_overlayColor[4] = hemorrhage;
	m_overlayColor[5] = lrnc;
	m_overlayColor[6] = lm;

	m_lookupTable = vtkSmartPointer<vtkLookupTable>::New();
	m_lookupTable->SetNumberOfTableValues(NUMBER_OF_COLOR);
	m_lookupTable->SetTableRange(0.0, 6.0);
	for (int i = 0; i < NUMBER_OF_COLOR; ++i) {
		double rgba[4];
		for (int j = 0; j < 4; ++j) {
			rgba[j] = m_overlayColor[i][j] / 255;
		}
		m_lookupTable->SetTableValue(i, rgba);
	}
	m_lookupTable->Build();

}

IADEOverlay::IADEOverlay(OverlayImageData::itkImageType::Pointer data, QObject * parent)
	:IADEOverlay(parent)
{
	m_data->Graft(data);
}

IADEOverlay::IADEOverlay(OverlayImageData * data, QObject * parent)
{
	m_data->ShallowCopy(data);
}

void IADEOverlay::updatedOverlay()
{
	Overlay::updatedOverlay();
	cout << "3" << endl;
	updateMeasurement3D();
	cout << "3" << endl;
	//cout << "2" << endl;
	//for( int i=m_data->GetExtent()[4]; i <= m_data->GetExtent()[5]; ++i)
	//	updateMeasurement2D(i);
	//cout << "2" << endl;

}

void IADEOverlay::updateMeasurement3D()
{
	// calculate the volume of a single voxel
	double voxelSize = 1;
	for (int i = 0; i < 3; ++i) {
		voxelSize *= m_data->GetSpacing()[i];
	}
	vtkSmartPointer<vtkImageAccumulate> imageAccumulate =
		vtkSmartPointer<vtkImageAccumulate>::New();
	imageAccumulate->SetInputData(m_data);
	imageAccumulate->SetComponentExtent(0, NUMBER_OF_COLOR - 1, 0, 0, 0, 0); // #LookupTable
	imageAccumulate->SetComponentOrigin(0, 0, 0);
	imageAccumulate->SetComponentSpacing(1, 0, 0);
	imageAccumulate->Update();
	for (int i = 1; i < NUMBER_OF_COLOR; ++i) {
		Measurements3D[i] = *static_cast<int*>(
			imageAccumulate->GetOutput()->GetScalarPointer(i, 0, 0)) * voxelSize;
		if (i == 1) {
			Measurements3D[0] = 0;
		}
		else if (i > 2) {
			Measurements3D[0] += Measurements3D[i];
		}
	}
	cerr << "Measurements3D" << endl;
	for (int i = 0; i < NUMBER_OF_COLOR; ++i) {
		cerr << Measurements3D[i] << ' ';
	}
	cerr << endl;
}

void IADEOverlay::updateMeasurement2D(int slice)
{
	// calculate the volume of a single voxel
	double pixelSize = 1;
	for (int i = 0; i < 2; ++i) {
		pixelSize *= m_data->GetSpacing()[i];
	}

	//vtkSmartPointer<vtkExtractVOI> extractVOI =
	//	vtkSmartPointer<vtkExtractVOI>::New();
	//extractVOI->SetInputData(m_data);
	//extractVOI->SetVOI(
	//	m_data->GetSpacing()[0],
	//	m_data->GetSpacing()[1],
	//	m_data->GetSpacing()[2],
	//	m_data->GetSpacing()[3],
	//	slice,
	//	slice);

	//extractVOI->Update();
	vtkSmartPointer<vtkImageAccumulate> imageAccumulate =
		vtkSmartPointer<vtkImageAccumulate>::New();
	imageAccumulate->SetInputData(m_data);
	imageAccumulate->SetComponentExtent(0, 1, 0, 0, 0, 0); // #LookupTable
	imageAccumulate->SetComponentOrigin(1, 0, 0);
	imageAccumulate->SetComponentSpacing(1, 0, 0);
	imageAccumulate->Update();
	for (int i = 1; i < 2; ++i) {
		Measurements2D[i] = *static_cast<int*>(
			imageAccumulate->GetOutput()->GetScalarPointer(i, 0, 0)) * pixelSize;
	}
}
