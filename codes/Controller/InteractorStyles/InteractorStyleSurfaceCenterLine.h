#ifndef __INTERACTOR_STYLE_SURFACE_CENTER_LINE_H__
#define __INTERACTOR_STYLE_SURFACE_CENTER_LINE_H__

#include "AbstractInteractorStyleSurface.h"
#include <vtkSmartPointer.h>


class vtkActor;
class vtkSeedWidget;

class InteractorStyleSurfaceCenterLine: 
	public AbstractInteractorStyleSurface
{
public:
	vtkTypeMacro(InteractorStyleSurfaceCenterLine, AbstractInteractorStyleSurface);
	static InteractorStyleSurfaceCenterLine* New();

	void SetCustomEnabled(bool flag);

protected:
	vtkSmartPointer<vtkActor> m_centerLineActor = nullptr;
	vtkSmartPointer<vtkSeedWidget> m_seedWidget = nullptr;

private:

};




#endif // !__INTERACTOR_STYLE_SURFACE_CENTER_LINE_H__