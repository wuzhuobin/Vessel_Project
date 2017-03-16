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

	//virtual void SetCenterLine(vtkPolyData* centerLine);
	//virtual vtkPolyData* GetCenterLine();

protected:
	AbstractSurfaceCenterLine();
	
	virtual ~AbstractSurfaceCenterLine() override;

	//virtual void VisualizeCenterLine(vtkPolyData* centerLine);
	/**
	 * whether centerline is created successful
	 * @return	true, succeed
	 *			false, failed
	 */
	virtual bool CreateCenterLine() = 0;
	
	//static vtkSmartPointer<vtkPolyData> m_centerLine;
	//vtkSmartPointer<vtkActor> m_centerLineActor = nullptr;


private:

};


#endif // !__ABSTRACT_SURFACE_CENTER_LINE_H__
