/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


The abstract interactor class used in medical viewers.
This class contains methods of image related processes, including changing slice position,
zoomming, dragging...etc.


Wong Matthew Lun
Copyright (C) 2016
*/

#ifndef __ABSTRACT_INTERACTOR_STYLE_SURFACE_H__
#define __ABSTRACT_INTERACTOR_STYLE_SURFACE_H__


#include "AbstractInteractorStyle.h"

#include <vtkInteractorStyleTrackballCamera.h>


//#ifndef SURFACE_VIEWER_CONSTITERATOR(METHOD)
//#define SURFACE_VIEWER_CONSTITERATOR(METHOD) \
//for(std::list<SurfaceViewer*>::const_iterator cit = \
//	m_synchronalViewers.cbegin(); cit != m_synchronalViewers.cend(); ++cit){\
//	(*cit)->##METHOD; \
//}
//#endif // !SURFACE_VIEWER_CONSTITERATOR(METHOD)

class SurfaceViewer;

class AbstractInteractorStyleSurface : public vtkInteractorStyleTrackballCamera,
	public AbstractInteractorStyle
{
public:
	vtkTypeMacro(AbstractInteractorStyleSurface, vtkInteractorStyleTrackballCamera);
	static AbstractInteractorStyleSurface *New();
	
	virtual SurfaceViewer* GetSurfaceViewer();

protected:
	AbstractInteractorStyleSurface();
	virtual ~AbstractInteractorStyleSurface();

	void OnMouseWheelForward();
	void OnMouseWheelBackward();
	void OnLeftButtonDown();
	void OnLeftButtonUp();
	void OnLeftDoubleClick();
	void OnRightButtonDown();
	void OnRightButtonUp();
	void OnRightDoubleClick();
	void OnMiddleButtonDown();
	void OnMiddleButtonUp();
	void OnMiddleDoubleClick();
	void OnMouseMove();
	void OnChar();
	void OnKeyPress();

	virtual double* GetOrigin();
	virtual double* GetSpacing();
	virtual int* GetExtent();


private:
	const static int RESET_PIXEL_DISTANCE = 5;
	bool CheckMoveDistance();
	int PreviousPosition[2] = { 0,0 };

};

#endif //__ABSTRACT_INTERACTOR_STYLE_SURFACE_H__