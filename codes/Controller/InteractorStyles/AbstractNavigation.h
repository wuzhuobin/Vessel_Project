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

#ifndef __ABSTRACT_NAVIGATION_H__
#define __ABSTRACT_NAVIGATION_H__


#include "AbstractInteractorStyleImage.h"


#ifndef MY_VIEWER_CONSTITERATOR(METHOD)
#define MY_VIEWER_CONSTITERATOR(METHOD) \
for(std::list<vtkImageViewer2*>::const_iterator cit = \
	AbstractInteractorStyleImage::m_synchronalViewers.cbegin(); \
	cit != AbstractInteractorStyleImage::m_synchronalViewers.cend(); ++cit){\
	ImageViewer* _viewer = dynamic_cast<ImageViewer*>(*cit);\
	if(_viewer != nullptr)\
		_viewer->##METHOD; \
}
#endif // !MY_VIEWER_CONSTITERATOR(METHOD)

class ImageViewer;

class AbstractNavigation : public AbstractInteractorStyleImage
{
public:
	vtkTypeMacro(AbstractNavigation, vtkInteractorStyleImage);
	static AbstractNavigation *New();
	
	virtual void SetImageViewer(ImageViewer* imageViewer);
	//virtual ImageViewer* GetImageViewer();
	virtual void SetCurrentSlice(int slice);
	virtual void SetCurrentFocalPointWithImageCoordinate(int* ijk);
	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);


protected:
	AbstractNavigation();
	~AbstractNavigation();

	virtual void AddSynchronalViewer(ImageViewer* imageViewer);
	
	virtual void OnMouseWheelForward();
	virtual void OnMouseWheelBackward();
	virtual void OnKeyPress();




	static std::list<ImageViewer*> m_synchronalViewers;
	ImageViewer* m_imageViewer;

private:
	void MoveSliceForward();
	void MoveSliceBackward();

	void SetCurrentFocalPointWithImageCoordinateByViewer(int i, int j, int k);
};

#endif //__ABSTRACT_NAVIGATION_H__