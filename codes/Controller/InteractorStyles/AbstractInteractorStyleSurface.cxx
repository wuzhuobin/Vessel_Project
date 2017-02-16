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

#include "AbstractInteractorStyleSurface.h"

#include <vtkCamera.h>
#include <vtkPropPicker.h>
#include <vtkRenderer.h>
#include <vtkImageData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkObjectFactory.h>

#include "SurfaceViewer.h"


vtkStandardNewMacro(AbstractInteractorStyleSurface);
std::list<SurfaceViewer*> AbstractInteractorStyleSurface::m_synchronalViewers;

AbstractInteractorStyleSurface::AbstractInteractorStyleSurface() : vtkInteractorStyle()
{
}

AbstractInteractorStyleSurface::~AbstractInteractorStyleSurface()
{
}

void AbstractInteractorStyleSurface::SetSurfaceViewer(SurfaceViewer * imageViewer)
{
	this->m_imageViewer = imageViewer;
	AddSynchronalViewer(imageViewer);
}

void AbstractInteractorStyleSurface::AddSynchronalViewer(SurfaceViewer * imageViewer)
{
	if (std::find(m_synchronalViewers.cbegin(), m_synchronalViewers.cend(), imageViewer)
		== m_synchronalViewers.cend()) {
		m_synchronalViewers.push_back(imageViewer);
	}
}

//void AbstractInteractorStyleSurface::SetCurrentSlice(int slice)
//{
//	VIEWER_CONSTITERATOR(SetSlice(slice));
//}

void AbstractInteractorStyleSurface::EnableSynchronalZooming(bool flag)
{
	this->m_synchronalZoomingFlag = flag;
	SynchronalZooming();
}

void AbstractInteractorStyleSurface::SynchronalZooming()
{
	if (!m_synchronalZoomingFlag)
		return;
	double scale = m_imageViewer->GetRenderer()->GetActiveCamera()->GetParallelScale();
	VIEWER_CONSTITERATOR(GetRenderer()->GetActiveCamera()->SetParallelScale(scale));
	VIEWER_CONSTITERATOR(Render());

	//for (std::list<vtkImageViewer2*>::iterator it = m_synchronalViewers.begin();
	//	it != m_synchronalViewers.end(); ++it) {
	//	(*it)->GetRenderer()->GetActiveCamera()->SetParallelScale(scale);
	//	(*it)->Render();
	//}
}
//
//int AbstractInteractorStyleSurface::GetSlice()
//{
//	return m_imageViewer->GetSlice();
//}
//
//int AbstractInteractorStyleSurface::GetMinSlice()
//{
//	return m_imageViewer->GetSliceMin();
//}
//
//int AbstractInteractorStyleSurface::GetMaxSlice()
//{
//	return m_imageViewer->GetSliceMax();
//}
//
//int AbstractInteractorStyleSurface::GetSliceOrientation()
//{
//	return m_imageViewer->GetSliceOrientation();
//}
//
//double * AbstractInteractorStyleSurface::GetOrigin()
//{
//	return m_imageViewer->GetInput()->GetOrigin();
//}
//
//double * AbstractInteractorStyleSurface::GetSpacing()
//{
//	return m_imageViewer->GetInput()->GetSpacing();
//}
//
//int * AbstractInteractorStyleSurface::GetExtent()
//{
//	return m_imageViewer->GetInput()->GetExtent();
//}

void AbstractInteractorStyleSurface::OnMouseWheelForward()
{
	vtkInteractorStyle::OnMouseWheelForward();
}

void AbstractInteractorStyleSurface::OnMouseWheelBackward()
{
	vtkInteractorStyle::OnMouseWheelBackward();
}

void AbstractInteractorStyleSurface::OnLeftButtonDown()
{
	this->m_numberOfLeftClicks++;

	// Reset numClicks - If mouse moved further than resetPixelDistance
	if (CheckMoveDistance())
	{
		this->m_numberOfLeftClicks = 1;
	}
	if (this->m_numberOfLeftClicks == 2)
	{
		OnLeftDoubleClick();
		this->m_numberOfLeftClicks = 0;
	}
	AbstractInteractorStyle::OnLeftButtonDown();
	//vtkInteractorStyleImage::OnLeftButtonDown();
}

void AbstractInteractorStyleSurface::OnLeftButtonUp()
{
	AbstractInteractorStyle::OnLeftButtonUp();
	//vtkInteractorStyleImage::OnLeftButtonUp();
}

void AbstractInteractorStyleSurface::OnLeftDoubleClick()
{
}

void AbstractInteractorStyleSurface::OnRightButtonDown()
{
	this->m_numberOfRightClicks++;

	// Reset numClicks - If mouse moved further than resetPixelDistance
	if (CheckMoveDistance())
	{
		this->m_numberOfRightClicks = 1;
	}


	if (this->m_numberOfRightClicks == 2)
	{
		OnRightDoubleClick();
		this->m_numberOfRightClicks = 0;
	}

	AbstractInteractorStyle::OnRightButtonDown();
	vtkInteractorStyle::OnRightButtonDown();
}

void AbstractInteractorStyleSurface::OnRightButtonUp()
{
	AbstractInteractorStyle::OnRightButtonUp();
	vtkInteractorStyle::OnRightButtonUp();
}

void AbstractInteractorStyleSurface::OnRightDoubleClick()
{
}

void AbstractInteractorStyleSurface::OnMiddleButtonDown()
{
	this->m_numberOfMiddleClicks++;

	// Reset numClicks - If mouse moved further than resetPixelDistance
	if (CheckMoveDistance())
	{
		this->m_numberOfMiddleClicks = 1;
	}


	if (this->m_numberOfMiddleClicks == 2)
	{
		OnMiddleDoubleClick();
		this->m_numberOfMiddleClicks = 0;
	}
	AbstractInteractorStyle::OnMiddleButtonDown();
	vtkInteractorStyle::OnMiddleButtonDown();
}

void AbstractInteractorStyleSurface::OnMiddleButtonUp()
{
	AbstractInteractorStyle::OnMiddleButtonUp();
	vtkInteractorStyle::OnMiddleButtonUp();
}

void AbstractInteractorStyleSurface::OnMiddleDoubleClick()
{
}

void AbstractInteractorStyleSurface::OnMouseMove()
{
	vtkInteractorStyle::OnMouseMove();
}

void AbstractInteractorStyleSurface::OnChar()
{
	vtkInteractorStyle::OnChar();
}

void AbstractInteractorStyleSurface::OnKeyPress()
{
	vtkInteractorStyle::OnKeyPress();
}

bool AbstractInteractorStyleSurface::CheckMoveDistance()
{
	int pickPosition[2];
	this->GetInteractor()->GetEventPosition(pickPosition);

	int xdist = pickPosition[0] - this->PreviousPosition[0];
	int ydist = pickPosition[1] - this->PreviousPosition[1];

	this->PreviousPosition[0] = pickPosition[0];
	this->PreviousPosition[1] = pickPosition[1];

	int moveDistance = (int)sqrt((double)(xdist*xdist + ydist*ydist));

	return (moveDistance <= RESET_PIXEL_DISTANCE);
}