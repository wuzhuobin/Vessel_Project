#ifndef __INTERACTOR_STYLE_SURFACE_CENTER_LINE_RESLICER_H__
#define __INTERACTOR_STYLE_SURFACE_CENTER_LINE_RESLICER_H__

#include "InteractorStyleSurfaceCenterLineSimpleClipping.h"

//class vtkSeedWidget;
class vtkHandleWidget;
class vtkKdTreePointLocator;
class vtkPolyData;
class vtkImageActor;
class InteractorStyleSurfaceCenterLineReslicerCallback;

class InteractorStyleSurfaceCenterLineReslicer :
	public InteractorStyleSurfaceCenterLineSimpleClipping
{
public:

	const static unsigned int NUM_OF_HANDLES = 1;

	vtkTypeMacro(InteractorStyleSurfaceCenterLineReslicer,
		InteractorStyleSurfaceCenterLineSimpleClipping);
	static InteractorStyleSurfaceCenterLineReslicer* New();

	virtual void SetCustomEnabled(bool flag) override;


protected:

	InteractorStyleSurfaceCenterLineReslicer();
	virtual ~InteractorStyleSurfaceCenterLineReslicer() override;

	//virtual void CreateCenterLine() override;
	virtual void InitializeHandleWidgets();
	virtual void InitializeReslicer();
	virtual void UpdateReslicer();
	//virtual void FindMaximumRadius();

	virtual void OnKeyPress() override;


	vtkSmartPointer<vtkKdTreePointLocator> m_pointLocator = nullptr;
	//vtkSmartPointer<vtkSeedWidget> m_seedWidget = nullptr;
	vtkSmartPointer<vtkHandleWidget> m_handleWidgets[NUM_OF_HANDLES] = {nullptr};
	vtkSmartPointer<vtkPolyData> m_triangulatedCenterLine = nullptr;
	vtkSmartPointer<vtkImageActor> m_imageActor = nullptr;

	//vtkSmartPointer<vtkTextActor> m_measurementText = nullptr;
	//vtkSmartPointer<vtkPolyData> m_centerLine = nullptr;

	friend class InteractorStyleSurfaceCenterLineReslicerCallback;

private:

};



#endif // !__INTERACTOR_STYLE_SURFACE_CENTER_LINE_RESLICER_H__
