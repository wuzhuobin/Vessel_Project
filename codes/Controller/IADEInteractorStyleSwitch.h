/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant
This class is based on vtkInteractorStyleSwitch, written to allow easy
switching between 2D interactors.
Wong Matthew Lun
Copyright (C) 2016
*/


#ifndef __IADE_INTERACTORSTYLE_SWITCH_H__
#define __IADE_INTERACTORSTYLE_SWITCH_H__

#include "InteractorStyleSwitch.h"

#include "QInteractorStyleNavigation.h"
#include "QInteractorStyleWindowLevel.h"
#include "QInteractorStyleWindowLevelThreshold.h"
#include "QInteractorStyleThreshold.h"
#include "QInteractorStylePaintBrush.h"
#include "QInteractorStyleVOI.h"
#include "QInteractorStyleLumenSeedsPlacer.h"
#include "QInteractorStyleVBDSmokerSeeds.h"
#include "QInteractorStyleTubularVOI.h"
#include "QInteractorStyleMaximumWallThickness.h"
#include "QInteractorStyleRuler.h"
#include "InteractorStylePolygonDraw.h"
#include "QInteractorStylePolygonDrawSeries.h"
#include "QInteractorStyleVesselSegmentation2.h"

class IADEInteractorStyleSwitch : public InteractorStyleSwitch
{
public:
	static IADEInteractorStyleSwitch* New();
	vtkTypeMacro(IADEInteractorStyleSwitch, InteractorStyleSwitch);

	vtkGetMacro(WindowLevel, QInteractorStyleWindowLevel*);
	SetInteractorStyleMacro(WindowLevel);
	CurrentStyleMacro(WindowLevel);

	vtkGetMacro(WindowLevelThreshold, QInteractorStyleWindowLevelThreshold*);
	SetInteractorStyleMacro(WindowLevelThreshold);
	CurrentStyleMacro(WindowLevelThreshold);

	vtkGetMacro(Threshold, QInteractorStyleThreshold*);
	SetInteractorStyleMacro(Threshold);
	CurrentStyleMacro(Threshold);

	vtkGetMacro(Navigation, QInteractorStyleNavigation*);
	SetInteractorStyleMacro(Navigation);
	CurrentStyleMacro(Navigation);

	vtkGetMacro(PaintBrush, QInteractorStylePaintBrush*);
	SetInteractorStyleMacro(PaintBrush);
	CurrentStyleMacro(PaintBrush);

	vtkGetMacro(LumenSeedsPlacer, QInteractorStyleLumenSeedsPlacer*);
	SetInteractorStyleMacro(LumenSeedsPlacer);
	CurrentStyleMacro(LumenSeedsPlacer);

	vtkGetMacro(VOI, QInteractorStyleVOI*);
	SetInteractorStyleMacro(VOI);
	CurrentStyleMacro(VOI);

	vtkGetMacro(TubularVOI, QInteractorStyleTubularVOI*);
	SetInteractorStyleMacro(TubularVOI);
	CurrentStyleMacro(TubularVOI);

	vtkGetMacro(Ruler, QInteractorStyleRuler*);
	SetInteractorStyleMacro(Ruler);
	CurrentStyleMacro(Ruler);

	vtkGetMacro(MaximumWallThickness, QInteractorStyleMaximumWallThickness*);
	SetInteractorStyleMacro(MaximumWallThickness);
	CurrentStyleMacro(MaximumWallThickness);

	vtkGetMacro(PolygonDraw, InteractorStylePolygonDraw*);
	SetInteractorStyleMacro(PolygonDraw);
	CurrentStyleMacro(PolygonDraw);

	vtkGetMacro(PolygonDrawSeries, QInteractorStylePolygonDrawSeries*);
	SetInteractorStyleMacro(PolygonDrawSeries);
	CurrentStyleMacro(PolygonDrawSeries);

	vtkGetMacro(VesselSegmentation2, QInteractorStyleVesselSegmentation2*);
	SetInteractorStyleMacro(VesselSegmentation2);
	CurrentStyleMacro(VesselSegmentation2);

	vtkGetMacro(VBDSmoker, QInteractorStyleVBDSmokerSeeds*);
	SetInteractorStyleMacro(VBDSmoker);
	CurrentStyleMacro(VBDSmoker);


protected:
	IADEInteractorStyleSwitch();

	QInteractorStyleNavigation*	Navigation;
	QInteractorStyleWindowLevel* WindowLevel;
	QInteractorStyleThreshold* Threshold;
	QInteractorStyleWindowLevelThreshold* WindowLevelThreshold;
	QInteractorStyleLumenSeedsPlacer* LumenSeedsPlacer;
	QInteractorStylePaintBrush*	PaintBrush;
	QInteractorStyleVOI* VOI;
	QInteractorStyleTubularVOI* TubularVOI;
	QInteractorStyleRuler* Ruler;
	QInteractorStyleMaximumWallThickness* MaximumWallThickness;
	InteractorStylePolygonDraw* PolygonDraw;
	QInteractorStylePolygonDrawSeries* PolygonDrawSeries;
	QInteractorStyleVesselSegmentation2* VesselSegmentation2;

	// IADE specified 
	QInteractorStyleVBDSmokerSeeds* VBDSmoker;

	//QInteractorStyleRuler* Ruler;
};



#endif