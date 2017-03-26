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
#include "QInteractorStylePaintBrush.h"
#include "QInteractorStyleVOI.h"
#include "QInteractorStyleLumenSeedsPlacer.h"
#include "QInteractorStyleVBDSmokerSeeds.h"
#include "QInteractorStyleTubularVOI.h"
#include "QInteractorStyleMaximumWallThickness.h"
#include "QInteractorStyleRuler.h"

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

	vtkGetMacro(VBDSmoker, QInteractorStyleVBDSmokerSeeds*);
	SetInteractorStyleMacro(VBDSmoker);
	CurrentStyleMacro(VBDSmoker);

protected:
	IADEInteractorStyleSwitch();

	QInteractorStyleNavigation*	Navigation;
	QInteractorStyleWindowLevel* WindowLevel;
	QInteractorStyleWindowLevelThreshold* WindowLevelThreshold;
	//QInteractorStyleVesselSegmentation* PolygonDraw;
	QInteractorStyleLumenSeedsPlacer* LumenSeedsPlacer;
	QInteractorStylePaintBrush*	PaintBrush;
	QInteractorStyleVOI* VOI;
	QInteractorStyleTubularVOI* TubularVOI;
	QInteractorStyleRuler* Ruler;
	QInteractorStyleMaximumWallThickness* MaximumWallThickness;

	// IADE specified 
	QInteractorStyleVBDSmokerSeeds* VBDSmoker;

	//QInteractorStyleRuler* Ruler;
};



#endif