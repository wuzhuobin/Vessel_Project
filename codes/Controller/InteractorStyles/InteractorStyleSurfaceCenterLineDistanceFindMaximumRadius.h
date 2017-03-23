#ifndef __INTERACTOR_STYLE_SURFACE_CENTER_LINE_FIND_MAXIMUM_RADIUS_H__
#define __INTERACTOR_STYLE_SURFACE_CENTER_LINE_FIND_MAXIMUM_RADIUS_H__

#include "InteractorStyleSurfaceCenterLineSimpleClipping.h"

//class vtkSeedWidget;
class vtkHandleWidget;
class vtkKdTreePointLocator;
class vtkPolyData;
class vtkTextActor;
class InteractorStyleSurfaceCenterLineDistanceFindMaximumRadiusCallback;

class InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius :
	public InteractorStyleSurfaceCenterLineSimpleClipping
{
public:

	const static unsigned int NUM_OF_HANDLES = 2;

	vtkTypeMacro(InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius,
		InteractorStyleSurfaceCenterLineSimpleClipping);
	static InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius* New();

	virtual void SetCustomEnabled(bool flag) override;
	virtual void FindMaximumRadius();


protected:

	InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius();
	virtual ~InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius() override;

	virtual void InitializeHandleWidgets();

	virtual void OnKeyPress() override;


	vtkSmartPointer<vtkKdTreePointLocator> m_trianglePointLocator = nullptr;
	vtkSmartPointer<vtkHandleWidget> m_handleWidgets[NUM_OF_HANDLES] = {nullptr};
	vtkSmartPointer<vtkPolyData> m_triangulatedCenterLine = nullptr;
	vtkSmartPointer<vtkTextActor> m_measurementText = nullptr;


private:

};



#endif // !__INTERACTOR_STYLE_SURFACE_CENTER_LINE_FIND_MAXIMUM_RADIUS_H__
