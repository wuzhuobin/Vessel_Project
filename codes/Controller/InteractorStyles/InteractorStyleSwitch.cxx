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
#include "InteractorStyleSwitch.h"

#include <vtkCommand.h>
#include <vtkCallbackCommand.h>	
#include <vtkObjectFactory.h>
#include <vtkInteractorStyle.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include "AbstractNavigation.h"

vtkStandardNewMacro(InteractorStyleSwitch);
//SET_CURRENT_STYLE_TO_MACRO_CPP(InteractorStyleTesting);

InteractorStyleSwitch::InteractorStyleSwitch()
{
	InteractorStyleTesting = vtkInteractorStyleImage::New();

	allStyles.push_back(InteractorStyleTesting);
	this->CurrentStyle = 0;
}

InteractorStyleSwitch::~InteractorStyleSwitch()
{
	/* Do not modifiy the lower code£¡ */
	for (std::list<vtkInteractorStyle*>::iterator it = allStyles.begin();
		it != allStyles.end(); ++it) {
		if ((*it) != NULL) {
			(*it)->Delete();
			(*it) = NULL;
		}
	}
	allStyles.clear();

}

void InteractorStyleSwitch::InternalUpdate(bool flag)
{

	// using enable update to call #EnabledOn() and #EnabledOff()
	AbstractInteractorStyle* _style = dynamic_cast<AbstractInteractorStyle*>(this->CurrentStyle);
	_style->SetCustomEnabled(flag);
	//for (std::list<vtkInteractorStyle*>::const_iterator cit = allStyles.cbegin();
	//	cit != allStyles.cend(); ++cit) {

	//	AbstractInteractorStyle* _style = dynamic_cast<AbstractInteractorStyle*>(*cit);
	//	if (_style != NULL) {
	//		if (this->CurrentStyle == *cit) {
	//			_style->CustomEnabledOn();
	//		}
	//		else {
	//			_style->CustomEnabledOff();
	//		}
	//	}

	//}
}

void InteractorStyleSwitch::SetAutoAdjustCameraClippingRange(int value)
{
	if (value == this->AutoAdjustCameraClippingRange)
	{
		return;
	}

	if (value < 0 || value > 1)
	{
		vtkErrorMacro("Value must be between 0 and 1 for" <<
			" SetAutoAdjustCameraClippingRange");
		return;
	}

	for (std::list<vtkInteractorStyle*>::iterator it = allStyles.begin();
		it != allStyles.end(); ++it) {
		(*it)->SetAutoAdjustCameraClippingRange(value);
	}
	this->Modified();
}

void InteractorStyleSwitch::SetDefaultRenderer(vtkRenderer* renderer)
{
	vtkInteractorStyle::SetDefaultRenderer(renderer);

	for (std::list<vtkInteractorStyle*>::const_iterator cit = allStyles.cbegin();
		cit != allStyles.cend(); ++cit) {
		(*cit)->SetDefaultRenderer(renderer);
	}
}

void InteractorStyleSwitch::SetCurrentRenderer(vtkRenderer* renderer)
{
	vtkInteractorStyle::SetCurrentRenderer(renderer);

	for (std::list<vtkInteractorStyle*>::const_iterator cit = allStyles.cbegin();
		cit != allStyles.cend(); ++cit) {
		(*cit)->SetCurrentRenderer(renderer);
	}
}

void InteractorStyleSwitch::SetImageViewer(ImageViewer* imageViewer)
{
	for (std::list<vtkInteractorStyle*>::const_iterator cit = allStyles.cbegin();
		cit != allStyles.cend(); ++cit) {
		AbstractNavigation* _style = 
			AbstractNavigation::SafeDownCast(*cit);
		if (_style != NULL) {
			_style->SetImageViewer(imageViewer);
		}
	}
}

void InteractorStyleSwitch::SetEnabled(int i)
{
	Superclass::SetEnabled(i);
	for (std::list<vtkInteractorStyle*>::const_iterator cit = allStyles.cbegin();
		cit != allStyles.cend(); ++cit) {
		(*cit)->SetEnabled(i);
	}
}

void InteractorStyleSwitch::SetEnabledOn()
{
	this->SetEnabled(1);
}

void InteractorStyleSwitch::SetEnabledOff()
{
	this->SetEnabled(0);
}


void InteractorStyleSwitch::PrintSelf(ostream & os, vtkIndent indent)
{
	vtkInteractorStyleSwitchBase::PrintSelf(os, indent);
}

void InteractorStyleSwitch::SetInteractor(vtkRenderWindowInteractor *iren)
{
	if (iren == this->Interactor)
	{
		return;
	}
	// if we already have an Interactor then stop observing it
	if (this->Interactor)
	{
		this->Interactor->RemoveObserver(this->EventCallbackCommand);
	}
	this->Interactor = iren;
	// add observers for each of the events handled in ProcessEvents
	if (iren)
	{
		iren->AddObserver(vtkCommand::CharEvent,
			this->EventCallbackCommand,
			this->Priority);

		iren->AddObserver(vtkCommand::DeleteEvent,
			this->EventCallbackCommand,
			this->Priority);
	}
}

