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


#ifndef INTERACTOR_STYLE_SWITCH_H
#define INTERACTOR_STYLE_SWITCH_H

#include <vtkSetGet.h>
#include <vtkInteractorStyleSwitchBase.h>

#include <list>

//#include "InteractorStyleNavigation.h"
//#include "InteractorStyleWindowLevel.h"
//#include "QInteractorStyleVesselSegmentation.h"
//#include "QInteractorStyleLumenSeedsPlacer.h"
//#include "QInteractorStylePaintBrush.h"
//#include "QInteractorStyleROI.h"
//#include "QInteractorStyleRuler.h"
#include "StyleSwitchMacro.h"

#ifndef SET_CURRENT_STYLE_TO_MACRO
#define SET_CURRENT_STYLE_TO_MACRO_H(style) \
void SetCurrentStyleTo##style();
#define SET_CURRENT_STYLE_TO_MACRO_CPP(style) \
void InteractorStyleSwitch::SetCurrentStyleTo##style(){ \
	if (this->CurrentStyle != ##style) { \
		if (this->CurrentStyle) { \
			this->CurrentStyle->SetInteractor(0); \
	} \
		this->CurrentStyle = this->##style;\
	} \
	if (this->CurrentStyle) { \
		this->CurrentStyle->SetInteractor(this->Interactor);\
		this->CurrentStyle->SetTDxStyle(this->TDxStyle);\
	} \
	this->InternalUpdate();\
}
#endif // !SET_CURRENT_STYLE_TO_MACRO


class ImageViewer;
class vtkInteractorStyleImage;
class InteractorStyleWindowLevel;
class InteractorStyleNavigation;

class InteractorStyleSwitch : public vtkInteractorStyleSwitchBase
{
public:
	static InteractorStyleSwitch* New();
	vtkTypeMacro(InteractorStyleSwitch, vtkInteractorStyleSwitchBase);
	virtual void PrintSelf(ostream& os, vtkIndent indent);

	vtkGetMacro(InteractorStyleTesting, vtkInteractorStyleImage*);
	vtkGetMacro(WindowLevel, InteractorStyleWindowLevel*);
	vtkGetMacro(Navigation, InteractorStyleNavigation*);
	//vtkGetMacro(PolygonDraw, QInteractorStyleVesselSegmentation*);
	//vtkGetMacro(SeedsPlacer, QInteractorStyleLumenSeedsPlacer*);
	//vtkGetMacro(PaintBrush, QInteractorStylePaintBrush*);
	//vtkGetMacro(ROI, QInteractorStyleROI*);
	//vtkGetMacro(Ruler, QInteractorStyleRuler*);

	void SetInteractor(vtkRenderWindowInteractor *iren);


	SET_CURRENT_STYLE_TO_MACRO_H(Navigation);
	SET_CURRENT_STYLE_TO_MACRO_H(WindowLevel);
	//SetInteractorStyleMacro(InteractorStyleTesting);
	//SetInteractorStyleMacro(Navigation);
	//void SetInteractorStyleToNavigation();
	//SetInteractorStyleMacro(WindowLevel);
	//SetInteractorStyleMacro(PolygonDraw);
	//SetInteractorStyleMacro(SeedsPlacer);
	//SetInteractorStyleMacro(PaintBrush);
	//SetInteractorStyleMacro(ROI);
	//SetInteractorStyleMacro(Ruler);

	//CurrentStyleMacro(InteractorStyleTesting);
	//CurrentStyleMacro(Navigation);
	//CurrentStyleMacro(WindowLevel);
	//CurrentStyleMacro(PolygonDraw);
	//CurrentStyleMacro(SeedsPlacer);
	//CurrentStyleMacro(PaintBrush);
	//CurrentStyleMacro(ROI);
	//CurrentStyleMacro(Ruler);

	virtual void SetDefaultRenderer(vtkRenderer* renderer);
	virtual void SetCurrentRenderer(vtkRenderer* renderer);
	virtual void SetImageViewer(ImageViewer* m_imageViewer);


	virtual void SetEnabled(int);
	virtual void SetEnabledOn();
	virtual void SetEnabledOff();

protected:
	InteractorStyleSwitch();
	~InteractorStyleSwitch();

	void InternalUpdate();
	void SetAutoAdjustCameraClippingRange(int value);

private:

	vtkInteractorStyleImage* InteractorStyleTesting;
	InteractorStyleNavigation*	Navigation;
	InteractorStyleWindowLevel* WindowLevel;
	//QInteractorStyleVesselSegmentation* PolygonDraw;
	//QInteractorStyleLumenSeedsPlacer* SeedsPlacer;
	//QInteractorStylePaintBrush*	PaintBrush;
	//QInteractorStyleROI* ROI;
	//QInteractorStyleRuler* Ruler;
	vtkInteractorStyle*	CurrentStyle;
	std::list<vtkInteractorStyle*> allStyles;
};



#endif