#ifndef __CENTERLINE_SURFACE_VIEWER_H__
#define __CENTERLINE_SURFACE_VIEWER_H__

#include "SurfaceViewer.h"

class vtkActor;
class vtkPolyDataMapper;
class vtkPolyData;
class CenterlineSurfaceViewer: public SurfaceViewer
{
public:

	static CenterlineSurfaceViewer* New();
	vtkTypeMacro(CenterlineSurfaceViewer, SurfaceViewer);
	void PrintSelf(ostream& os, vtkIndent indent);


	// Description:
	// Set/Get the input image to the viewer.
	virtual void SetCenterline(vtkPolyData* centerline);
	virtual vtkPolyData* GetCenterline();

	// Description:
	// Get the internal render window, renderer, image actor, and
	// image map instances.
	vtkGetObjectMacro(CenterlineActor, vtkActor);


protected:
	CenterlineSurfaceViewer();
	~CenterlineSurfaceViewer();

	vtkActor* CenterlineActor = nullptr;
	vtkPolyDataMapper* CenterlineMapper = nullptr;

	virtual void InstallPipeline();
	virtual void UnInstallPipeline();

private:

};


#endif // !__CENTERLINE_SURFACE_VIEWER_H__
