#ifndef __INTERACTOR_STYLE_SURFACE_CENTER_LINE_CURVED_NAVIGATION_H__
#define __INTERACTOR_STYLE_SURFACE_CENTER_LINE_CURVED_NAVIGATION_H__

#include "InteractorStyleSurfaceCenterLineSimpleClipping.h"

//class vtkSeedWidget;
class vtkHandleWidget;
class vtkKdTreePointLocator;
class vtkPolyData;
class vtkImageActor;
class vtkTextActor;
class InteractorStyleSurfaceCenterLineCurvedNavigationCallback;

class InteractorStyleSurfaceCenterLineCurvedNavigation :
	public InteractorStyleSurfaceCenterLineSimpleClipping
{
public:

	const static unsigned int NUM_OF_HANDLES = 1;

	vtkTypeMacro(InteractorStyleSurfaceCenterLineCurvedNavigation,
		InteractorStyleSurfaceCenterLineSimpleClipping);
	static InteractorStyleSurfaceCenterLineCurvedNavigation* New();

	virtual void SetCustomEnabled(bool flag) override;

	virtual void Update2DViewers();

	virtual void UpdateRadiusLabel();

protected:

	InteractorStyleSurfaceCenterLineCurvedNavigation();
	virtual ~InteractorStyleSurfaceCenterLineCurvedNavigation() override;

	virtual void InitializeHandleWidgets();

	virtual void OnKeyPress() override;


	vtkSmartPointer<vtkKdTreePointLocator> m_pointLocator = nullptr;
	vtkSmartPointer<vtkHandleWidget> m_handleWidgets[NUM_OF_HANDLES] = {nullptr};
	vtkSmartPointer<vtkTextActor> m_measurementText = nullptr;


private:

};



#endif // !__INTERACTOR_STYLE_SURFACE_CENTER_LINE_CURVED_NAVIGATION_H__
