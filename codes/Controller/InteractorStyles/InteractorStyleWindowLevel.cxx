/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
			Department of Imaging and Inteventional Radiology,
			Junior Research Assistant

Author:		Lok, Ka Hei Jason
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
			Department of Imaging and Inteventional Radiology,
			M.Phil Student

This class allows interactive modification to window and level.


Wong Matthew Lun, Lok Ka Hei
Copyright (C) 2016
*/

#include "InteractorStyleWindowLevel.h"

#include "ImageViewer.h"

#include <vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageProperty.h>
#include <vtkImageActor.h>
#include <vtkCommand.h>

vtkStandardNewMacro(InteractorStyleWindowLevel);

void InteractorStyleWindowLevel::SetWindow(double window)
{
	for (std::list<AbstractInteractorStyle*>::const_iterator cit =
		m_abstractInteractorStyles.cbegin();
		cit != m_abstractInteractorStyles.cend(); ++cit) {
		InteractorStyleWindowLevel* windowLevel =
			dynamic_cast<InteractorStyleWindowLevel*>(*cit);
		if (windowLevel != nullptr && windowLevel->m_customFlag &&
			m_imageViewer->GetInput() == windowLevel->m_imageViewer->GetInput()) {
			windowLevel->SetWindowByViewer(window);
		}
	}
}

void InteractorStyleWindowLevel::SetLevel(double level)
{
	for (std::list<AbstractInteractorStyle*>::const_iterator cit =
		m_abstractInteractorStyles.cbegin();
		cit != m_abstractInteractorStyles.cend(); ++cit) {
		InteractorStyleWindowLevel* windowLevel =
			dynamic_cast<InteractorStyleWindowLevel*>(*cit);
		if (windowLevel != nullptr && windowLevel->m_customFlag &&
			m_imageViewer->GetInput() == windowLevel->m_imageViewer->GetInput()) {
			windowLevel->SetLevelByViewer(level);
		}
	}
}

void InteractorStyleWindowLevel::SetWindowByViewer(double window)
{
	if (m_window != window) {
		m_window = window;
		m_imageViewer->GetImageActor()->GetProperty()->SetColorWindow(
			m_window);
		m_imageViewer->Render();
	}
}

void InteractorStyleWindowLevel::SetLevelByViewer(double level)
{
	if (m_level != level) {
		m_level = level;
		m_imageViewer->GetImageActor()->GetProperty()->SetColorLevel(
			m_level);
		m_imageViewer->Render();
	}
}

void InteractorStyleWindowLevel::SetWindowLevelByViewer(double window, double level)
{
	SetWindowByViewer(window);
	SetLevelByViewer(level);
}

InteractorStyleWindowLevel::InteractorStyleWindowLevel()
	:AbstractNavigation()
{
}

InteractorStyleWindowLevel::~InteractorStyleWindowLevel()
{

}

void InteractorStyleWindowLevel::StartWindowLevel()
{
	if (this->State != VTKIS_NONE)
	{
		return;
	}
	this->StartState(VTKIS_WINDOW_LEVEL);

	// Get the last (the topmost) image
	this->SetCurrentImageToNthImage(0);

	if (this->HandleObservers &&
		this->HasObserver(vtkCommand::StartWindowLevelEvent))
	{
		this->InvokeEvent(vtkCommand::StartWindowLevelEvent, this);
	}
	else
	{
		if (this->CurrentImageProperty)
		{
			vtkImageProperty *property = this->CurrentImageProperty;
			this->WindowLevelInitial[0] = property->GetColorWindow();
			this->WindowLevelInitial[1] = property->GetColorLevel();
		}
	}
}

void InteractorStyleWindowLevel::OnMouseMove()
{
	vtkInteractorStyleImage::OnMouseMove();
	AbstractNavigation::OnMouseMove();
}

void InteractorStyleWindowLevel::OnLeftButtonDown()
{
	vtkInteractorStyleImage::OnLeftButtonDown();
	AbstractNavigation::OnLeftButtonDown();
	//this->WindowLevelInitial[0] = m_imageViewer->GetImageActor()->
	//	GetProperty()->GetColorWindow();
	//this->WindowLevelInitial[1] = m_imageViewer->GetImageActor()->
	//	GetProperty()->GetColorLevel();
}

void InteractorStyleWindowLevel::OnLeftButtonUp()
{
	vtkInteractorStyleImage::OnLeftButtonUp();
	AbstractNavigation::OnLeftButtonUp();
}

void InteractorStyleWindowLevel::WindowLevel()
{
	vtkInteractorStyleImage::WindowLevel();
	m_window = CurrentImageProperty->GetColorWindow();
	m_level = CurrentImageProperty->GetColorLevel();
	SetWindow(m_window);
	SetLevel(m_level);
}

//void InteractorStyleWindowLevel::OnKeyPress()
//{
//	std::string key = this->Interactor->GetKeySym();
//// const double*  windowLevel = m_imageViewer->GetDefaultWindowLevel();
//	if (key == "r" || key == "R") {
//		SetWindowLevelByViewer(255,	127.5);
//		//AbstractNavigation::OnKeyPress();
//
//	}
//	else {
//		AbstractNavigation::OnKeyPress();
//	}
//
//}
//
//void InteractorStyleWindowLevel::OnKeyDown()
//{
//	std::string key = this->Interactor->GetKeySym();
//// const double*  windowLevel = m_imageViewer->GetDefaultWindowLevel();
//	if (key == "r" || key == "R") {
//		//AbstractNavigation::OnKeyPress();
//
//	}
//	else {
//		AbstractNavigation::OnKeyDown();
//	}
//}
//
//void InteractorStyleWindowLevel::OnKeyRelease()
//{
//	std::string key = this->Interactor->GetKeySym();
//// const double*  windowLevel = m_imageViewer->GetDefaultWindowLevel();
//	if (key == "r" || key == "R") {
//		//AbstractNavigation::OnKeyPress();
//
//	}
//	else {
//		AbstractNavigation::OnKeyRelease();
//	}
//}
//
//void InteractorStyleWindowLevel::OnKeyUp()
//{
//	std::string key = this->Interactor->GetKeySym();
//// const double*  windowLevel = m_imageViewer->GetDefaultWindowLevel();
//	if (key == "r" || key == "R") {
//		//AbstractNavigation::OnKeyPress();
//
//	}
//	else {
//		AbstractNavigation::OnKeyUp();
//	}
//}
//
void InteractorStyleWindowLevel::OnChar()
{
	//vtkInteractorStyleImage::OnChar();
	char key = this->Interactor->GetKeyCode();
	switch (key)
	{
	case 'r':
	case 'R':
		SetWindow(255);
		SetLevel(122.5);
		//AbstractNavigation::OnChar();
		//SynchronalZooming();
		break;
	default:
		AbstractNavigation::OnChar();
		break;
	}
}

