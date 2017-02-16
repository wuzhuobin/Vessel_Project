/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


This class is based on vtkInteractorStyleSwitch, written to allow easy
switching between 3D interactors.

Wong Matthew Lun
Copyright (C) 2016
*/


#ifndef INTERACTOR_STYLE_SWITCH_3D_H
#define INTERACTOR_STYLE_SWITCH_3D_H

//#include <string>
//#include <vtkCornerAnnotation.h>
//#include <vtkObjectFactory.h>
#include <vtkSetGet.h>
#include <vtkInteractorStyle.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkRenderer.h>
#include <vtkInteractorStyleSwitchBase.h>
#include <vtkInteractorStyleTrackballCamera.h>
//#include "AbstractInteractorStyle.h"
//#include "InteractorStyle3DNavigation.h"
//#include "InteractorStyle3DDistanceWidget.h"
//#include "StyleSwitchMacro.h"
#include <list>



#define SetInteractorStyle3DMacro(style) \
void SetInteractorStyleTo3D##style () { \
	if (CurrentStyle != ##style) { \
		if (this->CurrentStyle) { \
			this->InternalUpdate(false);\
			this->CurrentStyle->SetInteractor(0); \
	} \
		this->CurrentStyle = this->##style;\
	} \
	if (this->CurrentStyle) { \
		this->CurrentStyle->SetInteractor(this->Interactor);\
		this->CurrentStyle->SetTDxStyle(this->TDxStyle);\
		this->InternalUpdate(true);\
	} \
}

#define CurrentStyle3DMacro(style) \
bool CurrentStyleIs3D##style () {\
	return CurrentStyle == ##style; \
}

class SurfaceViewer;

class InteractorStyleSwitch3D : public vtkInteractorStyleSwitchBase
{
public:
	vtkTypeMacro(InteractorStyleSwitch3D, vtkInteractorStyleSwitchBase);
	static InteractorStyleSwitch3D* New();

	vtkGetMacro(TrackballCamera, vtkInteractorStyleTrackballCamera*);
	SetInteractorStyle3DMacro(TrackballCamera);
	CurrentStyle3DMacro(TrackballCamera);
	//vtkGetMacro(DistanceWidget, InteractorStyle3DDistanceWidget*);

	void SetInteractor(vtkRenderWindowInteractor *iren);

	//SetInteractorStyle3DMacro(DistanceWidget);

	//CurrentStyle3DMacro(DistanceWidget);

	virtual void SetSurfaceViewer(SurfaceViewer* surfaceViewer);
	virtual void SetDefaultRenderer(vtkRenderer* renderer);
	virtual void SetCurrentRenderer(vtkRenderer* renderer);
	virtual void OnChar();


	virtual void SetEnabled(int i);
	virtual void SetEnabledOn();
	virtual void SetEnabledOff();
	//void Change3DInteractionMode(vtkObject*, long unsigned int, void*);


protected:
	InteractorStyleSwitch3D();
	virtual ~InteractorStyleSwitch3D();

	void InternalUpdate(bool flag);
	void SetAutoAdjustCameraClippingRange(int value);

	//void Set3DMode(int);

	//vtkCornerAnnotation*						m_textActor;
	//vtkinteractorstyletrac
	vtkInteractorStyle*	CurrentStyle;
	vtkInteractorStyleTrackballCamera* TrackballCamera;
	std::list<vtkInteractorStyle*> allStyles;
	//InteractorStyle3DNavigation*		Navigation;
	//InteractorStyle3DDistanceWidget*	DistanceWidget;


};



#endif