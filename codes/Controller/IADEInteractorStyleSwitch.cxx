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
#include "IADEInteractorStyleSwitch.h"


#include <vtkCommand.h>
#include <vtkCallbackCommand.h>	
#include <vtkObjectFactory.h>
#include <vtkInteractorStyle.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

vtkStandardNewMacro(IADEInteractorStyleSwitch);


IADEInteractorStyleSwitch::IADEInteractorStyleSwitch()
{
	// The first instance of QInteractorStyleNavigation should have control 
	// of the ui
	Navigation = QInteractorStyleNavigation::New();
	allStyles.push_back(Navigation);
	WindowLevel = QInteractorStyleWindowLevelThreshold::New();
	allStyles.push_back(WindowLevel);
	//PolygonDraw = QInteractorStyleVesselSegmentation::New();
	LumenSeedsPlacer = QInteractorStyleLumenSeedsPlacer::New();
	allStyles.push_back(LumenSeedsPlacer);
	PaintBrush = QInteractorStylePaintBrush::New();
	allStyles.push_back(PaintBrush);
	VOI = QInteractorStyleVOI::New();
	allStyles.push_back(VOI);
	TubularVOI = QInteractorStyleTubularVOI::New();
	allStyles.push_back(TubularVOI);
	Ruler = QInteractorStyleMaximumWallThickness::New();
	allStyles.push_back(Ruler);

	// IADE specified
	VBDSmoker = QInteractorStyleVBDSmokerSeeds::New();
	allStyles.push_back(VBDSmoker);


	//allStyles.push_back(PolygonDraw);
	//allStyles.push_back(Ruler);

	this->CurrentStyle = 0;
}

//void IADEInteractorStyleSwitch::SetInteractorStyleToNavigation()
//{
//
//	if (!this->CurrentStyleIsNavigation()) {
//		if (this->CurrentStyle) {
//			this->CurrentStyle->SetInteractor(0);
//		}
//		this->CurrentStyle = (vtkInteractorStyle*) this->Navigation;
//	}
//	if (this->CurrentStyle) {
//		this->CurrentStyle->SetInteractor(this->Interactor);
//		this->CurrentStyle->SetTDxStyle(this->TDxStyle);
//	}
//	this->InternalUpdate();
//}

