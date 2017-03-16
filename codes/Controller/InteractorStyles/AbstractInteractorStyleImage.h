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

#ifndef __ABSTRACT_INTERACTOR_STYLE_IMAGE_H__
#define __ABSTRACT_INTERACTOR_STYLE_IMAGE_H__


#include "AbstractInteractorStyle.h"

#include <vtkInteractorStyleTrackballCamera.h>



//#ifndef VIEWER_CONSTITERATOR(METHOD)
//#define VIEWER_CONSTITERATOR(METHOD) \
//for(std::list<vtkImageViewer2*>::const_iterator cit = \
//	m_synchronalViewers.cbegin(); cit != m_synchronalViewers.cend(); ++cit){\
//	(*cit)->##METHOD; \
//}
//#endif // !VIEWER_CONSTITERATOR(METHOD)

class vtkImageViewer2;

class AbstractInteractorStyleImage : public vtkInteractorStyleTrackballCamera, 
	public AbstractInteractorStyle
{
public:
	vtkTypeMacro(AbstractInteractorStyleImage, vtkInteractorStyleTrackballCamera);
	static AbstractInteractorStyleImage *New();
	
	virtual vtkImageViewer2* GetVtkImageViewer2();
	virtual void SetCurrentSlice(int slice);
	virtual void EnableSynchronalZooming(bool flag);

protected:
	AbstractInteractorStyleImage();
	virtual ~AbstractInteractorStyleImage();

	virtual void SynchronalZooming();
	
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


	virtual int GetWindow();
	virtual int GetLevel();
	virtual int GetSlice();
	virtual int GetMinSlice();
	virtual int GetMaxSlice();
	virtual int GetSliceOrientation();
	virtual double* GetOrigin();
	virtual double* GetSpacing();
	virtual int* GetExtent();


private:
	const static int RESET_PIXEL_DISTANCE = 5;
	bool CheckMoveDistance();
	bool m_synchronalZoomingFlag = true;
	int PreviousPosition[2] = { 0 };

};

#endif //__ABSTRACT_INTERACTOR_STYLE_IMAGE_H__