#ifndef __ABSTRACT_SURFACE_CENTER_LINE_H__
#define __ABSTRACT_SURFACE_CENTER_LINE_H__

#include "AbstractInteractorStyleSurface.h"

#include <vtkSmartPointer.h>

class vtkPolyData;
class vtkActor;
class CenterlineSurfaceViewer;

class AbstractSurfaceCenterLine: public AbstractInteractorStyleSurface
{
public:
	vtkTypeMacro(AbstractSurfaceCenterLine, AbstractInteractorStyleSurface);

	virtual void SetCustomEnabled(bool flag);

	virtual CenterlineSurfaceViewer* GetCenterlineSurfaceViewer();

	/**
	* whether centerline is created successful
	* @return	true, succeed
	*			false, failed
	*/
	virtual bool CreateCenterLine() = 0;

protected:
	AbstractSurfaceCenterLine();
	
	virtual ~AbstractSurfaceCenterLine() override;


	

private:

};


#endif // !__ABSTRACT_SURFACE_CENTER_LINE_H__
