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
	m_window = window;
	for (std::list<ImageViewer*>::iterator it = m_synchronalViewers.begin();
		it != m_synchronalViewers.end(); ++it) {
		// using the input address to figure out whether they are the same image
		/*if (m_imageViewer->GetOriginalInput() == (*it)->GetOriginalInput())*/ {
			(*it)->GetImageActor()->GetProperty()->SetColorWindow(
				m_window);
			(*it)->GetImageActor()->GetProperty()->SetColorLevel(
				m_level);
			(*it)->Render();
		}

	}
}

void InteractorStyleWindowLevel::SetLevel(double level)
{
	m_level = level;
	for (std::list<ImageViewer*>::iterator it = m_synchronalViewers.begin();
		it != m_synchronalViewers.end(); ++it) {
		// using the input address to figure out whether they are the same image
		if (m_imageViewer->GetInput() == (*it)->GetInput()) {
			(*it)->GetImageActor()->GetProperty()->SetColorWindow(
				m_window);
			(*it)->GetImageActor()->GetProperty()->SetColorLevel(
				m_level);
			(*it)->Render();
		}

	}
}

void InteractorStyleWindowLevel::SetWindowLevel(double window, double level)
{
	m_window = window;
	m_level = level;
	for (std::list<ImageViewer*>::iterator it = m_synchronalViewers.begin();
		it != m_synchronalViewers.end(); ++it) {
		// using the input address to figure out whether they are the same image
		if (m_imageViewer->GetInput() == (*it)->GetInput()) {
			(*it)->GetImageActor()->GetProperty()->SetColorWindow(
				m_window);
			(*it)->GetImageActor()->GetProperty()->SetColorLevel(
				m_level);
			(*it)->Render();
		}

	}

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
	SetWindowLevel(CurrentImageProperty->GetColorWindow(),
		CurrentImageProperty->GetColorLevel());
}

//void InteractorStyleWindowLevel::OnKeyPress()
//{
//	std::string key = this->Interactor->GetKeySym();
//// const double*  windowLevel = m_imageViewer->GetDefaultWindowLevel();
//	if (key == "r" || key == "R") {
//		SetWindowLevel(255,	127.5);
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
		SetWindowLevel(255,
			123);
		//AbstractNavigation::OnChar();
		//SynchronalZooming();
		break;
	default:
		AbstractNavigation::OnChar();
		break;
	}
}