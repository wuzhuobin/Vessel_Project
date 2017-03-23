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
#include "QInteractorStyleWindowLevelThreshold.h"
#include "QInteractorStylePaintBrush.h"
#include "QInteractorStyleVOI.h"
#include "QInteractorStyleLumenSeedsPlacer.h"
#include "QInteractorStyleVBDSmokerSeeds.h"
#include "QInteractorStyleTubularVOI.h"
#include "QInteractorStyleMaximumWallThickness.h"

class IADEInteractorStyleSwitch : public InteractorStyleSwitch
{
public:
	static IADEInteractorStyleSwitch* New();
	vtkTypeMacro(IADEInteractorStyleSwitch, InteractorStyleSwitch);


	vtkGetMacro(WindowLevel, QInteractorStyleWindowLevelThreshold*);
	SetInteractorStyleMacro(WindowLevel);
	CurrentStyleMacro(WindowLevel);

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

	vtkGetMacro(Ruler, QInteractorStyleMaximumWallThickness*);
	SetInteractorStyleMacro(Ruler);
	CurrentStyleMacro(Ruler);

	vtkGetMacro(VBDSmoker, QInteractorStyleVBDSmokerSeeds*);
	SetInteractorStyleMacro(VBDSmoker);
	CurrentStyleMacro(VBDSmoker);

protected:
	IADEInteractorStyleSwitch();

	QInteractorStyleNavigation*	Navigation;
	QInteractorStyleWindowLevelThreshold* WindowLevel;
	//QInteractorStyleVesselSegmentation* PolygonDraw;
	QInteractorStyleLumenSeedsPlacer* LumenSeedsPlacer;
	QInteractorStylePaintBrush*	PaintBrush;
	QInteractorStyleVOI* VOI;
	QInteractorStyleTubularVOI* TubularVOI;
	QInteractorStyleMaximumWallThickness* Ruler;


	// IADE specified 
	QInteractorStyleVBDSmokerSeeds* VBDSmoker;

	//QInteractorStyleMaximumWallThickness* Ruler;
};



#endif