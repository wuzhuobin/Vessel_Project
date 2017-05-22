#include "PlaqueQuantMeasurement.h"

#include "MaximumWallThickness2.h"
#include "InteractorStyleSurfaceCenterLineStenosis.h"
#include "ImageSliceBySliceAccumulate.h"

#include <vtkLookupTable.h>
#include <vtkImageAccumulate.h>
#include <vtkExtractVOI.h>
#include <vtkPolyData.h>
#include <vtkFieldData.h>
PlaqueQuantMeasurement::PlaqueQuantMeasurement(QObject * parent)
	: Measurement(parent)
{
}

PlaqueQuantMeasurement::~PlaqueQuantMeasurement()
{
}

void PlaqueQuantMeasurement::update()
{
	updateMeasurement3D();
	updateMeasurement2D();


	Measurement::update();
}

void PlaqueQuantMeasurement::updateMeasurement2D(int slice)
{
	//if (overlayMTime2D != static_cast<PlaqueQuantOverlay*>(m_overlay)->getData()->GetMTime()) {
	//	overlayMTime2D = static_cast<PlaqueQuantOverlay*>(m_overlay)->getData()->GetMTime();
	//	m_measurements2DMap.clear();

	//}
	if (!m_measurements2DMap.contains(slice))
	{
		// calculate the volume of a single voxel
		double pixelSize = 1;
		for (int i = 0; i < 2; ++i) {
			pixelSize *= static_cast<PlaqueQuantOverlay*>(m_overlay)->getData()->GetSpacing()[i];
		}
		if (slice > static_cast<PlaqueQuantOverlay*>(m_overlay)->getData()->GetExtent()[5] ||
			slice < static_cast<PlaqueQuantOverlay*>(m_overlay)->getData()->GetExtent()[4]) {
			//qDebug() << "error";
			//qDebug() << "out of extent";
			return;
		}
		vtkSmartPointer<vtkExtractVOI> extractVOI =
			vtkSmartPointer<vtkExtractVOI>::New();
		extractVOI->SetInputData(static_cast<PlaqueQuantOverlay*>(m_overlay)->getData());
		extractVOI->SetVOI(
			static_cast<PlaqueQuantOverlay*>(m_overlay)->getData()->GetExtent()[0],
			static_cast<PlaqueQuantOverlay*>(m_overlay)->getData()->GetExtent()[1],
			static_cast<PlaqueQuantOverlay*>(m_overlay)->getData()->GetExtent()[2],
			static_cast<PlaqueQuantOverlay*>(m_overlay)->getData()->GetExtent()[3],
			slice,
			slice);

		vtkSmartPointer<vtkImageAccumulate> imageAccumulate =
			vtkSmartPointer<vtkImageAccumulate>::New();
		imageAccumulate->SetInputConnection(extractVOI->GetOutputPort());
		imageAccumulate->SetComponentExtent(0, 1, 0, 0, 0, 0); // #LookupTable
		imageAccumulate->SetComponentOrigin(1, 0, 0);
		imageAccumulate->SetComponentSpacing(1, 0, 0);
		imageAccumulate->Update();

		m_measurements2DMap.insert(slice, QSharedPointer<double>(new double[4], std::default_delete<double[]>()));
		for (int i = 0; i < 2; ++i) {
			m_measurements2DMap[slice].data()[i] = *static_cast<int*>(
				imageAccumulate->GetOutput()->GetScalarPointer(i, 0, 0)) * pixelSize;
		}
		// NMI calculation
		m_measurements2DMap[slice].data()[2] = m_measurements2DMap[slice].data()[0] /
			(m_measurements2DMap[slice].data()[0] + m_measurements2DMap[slice].data()[1]);
		cerr << "Measurements2D" << endl;
		for (int i = 0; i < 3; ++i) {
			cerr << m_measurements2DMap[slice].data()[i] << ' ';
		}
		cerr << endl;

		//try
		//{
		//	vtkSmartPointer<MaximumWallThickness> mwt =
		//		vtkSmartPointer<MaximumWallThickness>::New();
		//	mwt->SetSliceImage(extractVOI->GetOutput());
		//	mwt->SetLumemIntensity(2);
		//	mwt->SetVesselIntensity(1);
		//	mwt->Update();
		//	m_measurements2DMap[slice].data()[3] = mwt->GetDistanceLoopPairVect().at(0).Distance;
		//}
		//catch (...)
		//{
		//	m_measurements2DMap[slice].data()[3] = -1;
		//}
	}
	//std::copy_n(m_measurements2DMap[slice].data(), 4, m_measurements2D);
}

void PlaqueQuantMeasurement::updateMeasurement2D()
{
	//if (overlayMTime2D == static_cast<PlaqueQuantOverlay*>(m_overlay)->getData()->GetMTime()) {
	//	return;
	//}
	//overlayMTime2D = static_cast<PlaqueQuantOverlay*>(m_overlay)->getData()->GetMTime();
	// calculate the volume of a single voxel
	double pixelSize = 1;
	for (int i = 0; i < 2; ++i) {
		pixelSize *= static_cast<PlaqueQuantOverlay*>(m_overlay)->getData()->GetSpacing()[i];
	}
	vtkSmartPointer<ImageSliceBySliceAccumulate> imageSliceBySliceAccumulate =
		vtkSmartPointer<ImageSliceBySliceAccumulate>::New();
	imageSliceBySliceAccumulate->SetInputData(static_cast<PlaqueQuantOverlay*>(m_overlay)->getData());
	imageSliceBySliceAccumulate->SetExtent(0, 1, 0, 0, 0, 0); // #LookupTable
	imageSliceBySliceAccumulate->SetOrigin(1, 0, 0);
	imageSliceBySliceAccumulate->SetSpacing(1, 0, 0);
	imageSliceBySliceAccumulate->Update();


	vtkSmartPointer<MaximumWallThickness2> mwt =
		vtkSmartPointer<MaximumWallThickness2>::New();
	mwt->SetInputData(static_cast<PlaqueQuantOverlay*>(m_overlay)->getData());
	mwt->SetLumenIntensity(2);
	mwt->SetVesselWallIntensity(1);
	mwt->Update();

	for (int j = static_cast<PlaqueQuantOverlay*>(m_overlay)->getData()->GetExtent()[4];
		j <= static_cast<PlaqueQuantOverlay*>(m_overlay)->getData()->GetExtent()[5]; j++)
	{
		m_measurements2DMap.insert(j, QSharedPointer<double>(new double[4], std::default_delete<double[]>()));
		for (int i = 0; i < 2; ++i) {
			m_measurements2DMap[j].data()[i] = *static_cast<int*>(
				imageSliceBySliceAccumulate->GetOutput()->GetScalarPointer(i, j, 0)) * pixelSize;
		}
		m_measurements2DMap[j].data()[3] = mwt->MaxDistances[j];
		// NMI calculation
		m_measurements2DMap[j].data()[2] = m_measurements2DMap[j].data()[0] /
			(m_measurements2DMap[j].data()[0] + m_measurements2DMap[j].data()[1]);
	}
}

void PlaqueQuantMeasurement::updateStenosis(double stenosis)
{
	m_stenosis = stenosis;
}

//
//void PlaqueQuantMeasurement::setSlice(int slice)
//{
//	m_currentSlice = slice;
//	PlaqueQuantMeasurement::update();
//}

void PlaqueQuantMeasurement::updateMeasurement3D()
{
	//if (overlayMTime3D == static_cast<PlaqueQuantOverlay*>(m_overlay)->getData()->GetMTime()) {
	//	return;
	//}
	//overlayMTime3D = static_cast<PlaqueQuantOverlay*>(m_overlay)->getData()->GetMTime();
	// calculate the volume of a single voxel
	double voxelSize = 1;
	for (int i = 0; i < 3; ++i) {
		voxelSize *= static_cast<PlaqueQuantOverlay*>(m_overlay)->getData()->GetSpacing()[i];
	}
	vtkSmartPointer<vtkImageAccumulate> imageAccumulate =
		vtkSmartPointer<vtkImageAccumulate>::New();
	imageAccumulate->SetInputData(static_cast<PlaqueQuantOverlay*>(m_overlay)->getData());
	imageAccumulate->SetComponentExtent(0, PlaqueQuantOverlay::NUMBER_OF_COLOR - 1, 0, 0, 0, 0); // #LookupTable
	imageAccumulate->SetComponentOrigin(0, 0, 0);
	imageAccumulate->SetComponentSpacing(1, 0, 0);
	imageAccumulate->Update();
	for (int i = 1; i < PlaqueQuantOverlay::NUMBER_OF_COLOR; ++i) {
		m_measurements3D[i] = *static_cast<int*>(
			imageAccumulate->GetOutput()->GetScalarPointer(i, 0, 0)) * voxelSize;
		if (i == 1) {
			// initialize total plaque
			m_measurements3D[0] = 0;
		}
		else if (i > 2) {
			// total plaque
			m_measurements3D[0] += m_measurements3D[i];
		}
	}
	//cerr << "Measurements3D" << endl;
	//for (int i = 0; i < PlaqueQuantOverlay::NUMBER_OF_COLOR; ++i) {
	//	cerr << Measurements3D[i] << ' ';
	//}
	//cerr << endl;
}
