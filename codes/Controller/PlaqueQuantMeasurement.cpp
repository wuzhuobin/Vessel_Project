#include "PlaqueQuantMeasurement.h"

#include "MaximumWallThickness.h"
#include "InteractorStyleSurfaceCenterLineStenosis.h"

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

PlaqueQuantOverlay * PlaqueQuantMeasurement::getPlaqueQuantOverlay()
{
	return static_cast<PlaqueQuantOverlay*>(m_overlay);
}

void PlaqueQuantMeasurement::update()
{
	updateMeasurement3D();
	updateMeasurement2D(m_currentSlice);
	updateStenosis();



	Measurement::update();
}

void PlaqueQuantMeasurement::updateMeasurement2D(int slice)
{
	if (overlayMTime2D != getPlaqueQuantOverlay()->getData()->GetMTime()) {
		overlayMTime2D = getPlaqueQuantOverlay()->getData()->GetMTime();
		m_measurements2DMap.clear();

	}
	if (!m_measurements2DMap.contains(slice))
	{
		// calculate the volume of a single voxel
		double pixelSize = 1;
		for (int i = 0; i < 2; ++i) {
			pixelSize *= getPlaqueQuantOverlay()->getData()->GetSpacing()[i];
		}
		if (slice > getPlaqueQuantOverlay()->getData()->GetExtent()[5] ||
			slice < getPlaqueQuantOverlay()->getData()->GetExtent()[4]) {
			//qDebug() << "error";
			//qDebug() << "out of extent";
			return;
		}
		vtkSmartPointer<vtkExtractVOI> extractVOI =
			vtkSmartPointer<vtkExtractVOI>::New();
		extractVOI->SetInputData(getPlaqueQuantOverlay()->getData());
		extractVOI->SetVOI(
			getPlaqueQuantOverlay()->getData()->GetExtent()[0],
			getPlaqueQuantOverlay()->getData()->GetExtent()[1],
			getPlaqueQuantOverlay()->getData()->GetExtent()[2],
			getPlaqueQuantOverlay()->getData()->GetExtent()[3],
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

		try
		{
			vtkSmartPointer<MaximumWallThickness> mwt =
				vtkSmartPointer<MaximumWallThickness>::New();
			mwt->SetSliceImage(extractVOI->GetOutput());
			mwt->SetLumemIntensity(2);
			mwt->SetVesselIntensity(1);
			mwt->Update();
			m_measurements2DMap[slice].data()[3] = mwt->GetDistanceLoopPairVect().at(0).Distance;
		}
		catch (...)
		{
			m_measurements2DMap[slice].data()[3] = -1;
		}
	}
	std::copy_n(m_measurements2DMap[slice].data(), 4, m_measurements2D);
}

void PlaqueQuantMeasurement::updateStenosis()
{
	vtkDataArray* stenosis = getPlaqueQuantOverlay()->getCenterLine()->GetFieldData()->GetArray(InteractorStyleSurfaceCenterLineStenosis::STENOSIS);
	if (stenosis) {
		m_stenosis = (stenosis->GetTuple1(0));
	}
}

void PlaqueQuantMeasurement::setSlice(int slice)
{
	m_currentSlice = slice;
	//updateMeasurement2D(m_currentSlice);
	PlaqueQuantMeasurement::update();
}

void PlaqueQuantMeasurement::updateMeasurement3D()
{
	if (overlayMTime3D == getPlaqueQuantOverlay()->getData()->GetMTime()) {
		return;
	}
	overlayMTime3D = getPlaqueQuantOverlay()->getData()->GetMTime();
	// calculate the volume of a single voxel
	double voxelSize = 1;
	for (int i = 0; i < 3; ++i) {
		voxelSize *= getPlaqueQuantOverlay()->getData()->GetSpacing()[i];
	}
	vtkSmartPointer<vtkImageAccumulate> imageAccumulate =
		vtkSmartPointer<vtkImageAccumulate>::New();
	imageAccumulate->SetInputData(getPlaqueQuantOverlay()->getData());
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
