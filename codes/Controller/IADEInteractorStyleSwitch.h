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
#include "QInteractorStylePaintBrush.h"
#include "QInteractorStyleLumenSeedsPlacer.h"

class IADEInteractorStyleSwitch : public InteractorStyleSwitch
{
public:
	static IADEInteractorStyleSwitch* New();
	vtkTypeMacro(IADEInteractorStyleSwitch, InteractorStyleSwitch);


	vtkGetMacro(WindowLevel, QInteractorStyleWindowLevel*);
	SetInteractorStyleMacro(WindowLevel);
	CurrentStyleMacro(WindowLevel);

	vtkGetMacro(Navigation, QInteractorStyleNavigation*);
	SetInteractorStyleMacro(Navigation);
	CurrentStyleMacro(Navigation);
	//vtkGetMacro(PolygonDraw, QInteractorStyleVesselSegmentation*);
	vtkGetMacro(PaintBrush, QInteractorStylePaintBrush*);
	SetInteractorStyleMacro(PaintBrush);
	CurrentStyleMacro(PaintBrush);

	vtkGetMacro(LumenSeedsPlacer, QInteractorStyleLumenSeedsPlacer*);
	SetInteractorStyleMacro(LumenSeedsPlacer);
	CurrentStyleMacro(LumenSeedsPlacer);
	//vtkGetMacro(ROI, QInteractorStyleROI*);
	//vtkGetMacro(Ruler, QInteractorStyleRuler*);


	//SetInteractorStyleMacro(InteractorStyleTesting);
	//SetInteractorStyleMacro(Navigation);
	//void SetInteractorStyleToNavigation();
	//SetInteractorStyleMacro(WindowLevel);
	//SetInteractorStyleMacro(PolygonDraw);
	//SetInteractorStyleMacro(ROI);
	//SetInteractorStyleMacro(Ruler);

	//CurrentStyleMacro(InteractorStyleTesting);
	//CurrentStyleMacro(Navigation);
	//CurrentStyleMacro(WindowLevel);
	//CurrentStyleMacro(PolygonDraw);
	//CurrentStyleMacro(ROI);
	//CurrentStyleMacro(Ruler);

protected:
	IADEInteractorStyleSwitch();

	QInteractorStyleNavigation*	Navigation;
	QInteractorStyleWindowLevel* WindowLevel;
	//QInteractorStyleVesselSegmentation* PolygonDraw;
	QInteractorStyleLumenSeedsPlacer* LumenSeedsPlacer;
	QInteractorStylePaintBrush*	PaintBrush;
	//QInteractorStyleROI* ROI;
	//QInteractorStyleRuler* Ruler;
};



#endif