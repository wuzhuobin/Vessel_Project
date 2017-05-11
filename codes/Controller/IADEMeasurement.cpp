#include "IADEMeasurement.h"

#include "QInteractorStyleSurfaceCenterLineICDAStandard.h"
#include "QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter.h"
#include "QInteractorStyleSurfaceCenterLineVBDUboguStandard.h"
#include "QInteractorStyleVBDSmokerSeeds.h"

IADEMeasurement::IADEMeasurement(QObject * parent)
{
}

IADEMeasurement::~IADEMeasurement()
{
}

void IADEMeasurement::updateICDAStandard()
{
}

void IADEMeasurement::updateVBDSmoker()
{
}

void IADEMeasurement::updateVBDUbogu()
{
}

void IADEMeasurement::update()
{
	updateICDAStandard();
	updateVBDSmoker();
	updateVBDUbogu();

	Measurement::update();
}