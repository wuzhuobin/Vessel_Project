#ifndef __INTERACTOR_STYLE_SURFACE_CENTER_LINE_H__
#define __INTERACTOR_STYLE_SURFACE_CENTER_LINE_H__

#include "AbstractInteractorStyleSurface.h"
#include <vtkSmartPointer.h>


class vtkActor;
class vtkSeedWidget;
class vtkPolyData;
class vtkKdTreePointLocator;
class vtkIdList;

class InteractorStyleSurfaceCenterLine: 
	public AbstractInteractorStyleSurface
{
public:

	const static unsigned int NUM_OF_SEEDS = 2;
	vtkTypeMacro(InteractorStyleSurfaceCenterLine, AbstractInteractorStyleSurface);
	static InteractorStyleSurfaceCenterLine* New();

	void SetCustomEnabled(bool flag);

protected:

	virtual void OnKeyPress();

	vtkSmartPointer<vtkIdList> m_sourceIds = nullptr;
	vtkSmartPointer<vtkIdList> m_targetIds = nullptr;
	vtkSmartPointer<vtkKdTreePointLocator> m_pointLocator = nullptr;
	vtkSmartPointer<vtkPolyData> m_centerLine = nullptr;
	vtkSmartPointer<vtkActor> m_centerLineActor = nullptr;
	vtkSmartPointer<vtkSeedWidget> m_seedWidget = nullptr;

private:

};




#endif // !__INTERACTOR_STYLE_SURFACE_CENTER_LINE_H__