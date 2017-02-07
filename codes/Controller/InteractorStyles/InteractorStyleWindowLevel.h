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

#ifndef INTERACTOR_STYLE_WINDOW_LEVEL_H
#define INTERACTOR_STYLE_WINDOW_LEVEL_H
#include "AbstractNavigation.h"


class InteractorStyleWindowLevel : public AbstractNavigation
{
public:
	vtkTypeMacro(InteractorStyleWindowLevel, AbstractNavigation);
	static InteractorStyleWindowLevel* New();

	virtual void SetWindow(double window);
	virtual void SetLevel(double level);
	virtual void WindowLevel();

protected:
	InteractorStyleWindowLevel();
	~InteractorStyleWindowLevel();

	/**
	 * @override
	 * almost the same as #vtkInteractorStyleImage, except for specifying vtkImageActor
	 * by using #SetCurrentImageToNthImage() .
	 */
	virtual void StartWindowLevel();

	virtual void OnMouseMove();
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	//virtual void OnKeyPress();
	//virtual void OnKeyDown();
	//virtual void OnKeyRelease();
	//virtual void OnKeyUp();
	virtual void OnChar();

	virtual void SetWindowByViewer(double window);
	virtual void SetLevelByViewer(double level);
	virtual void SetWindowLevelByViewer(double window, double level);

	double m_window;
	double m_level;

	

};



#endif

