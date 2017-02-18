#ifndef __INTERACTOR_STYLE_SURFACE_CENTER_LINE_FIND_MAXIMUM_RADIUS_H__
#define __INTERACTOR_STYLE_SURFACE_CENTER_LINE_FIND_MAXIMUM_RADIUS_H__

#include "InteractorStyleSurfaceCenterLineSimpleClipping.h"

class vtkSeedWidget;
class vtkKdTreePointLocator;
class vtkPolyData;
class vtkTextActor;


class InteractorStyleSurfaceCenterLineFindMaximumRadius :
	public InteractorStyleSurfaceCenterLineSimpleClipping
{
public:

	const static unsigned int NUM_OF_SEEDS = 2;

	vtkTypeMacro(InteractorStyleSurfaceCenterLineFindMaximumRadius,
		InteractorStyleSurfaceCenterLineSimpleClipping);
	static InteractorStyleSurfaceCenterLineFindMaximumRadius* New();

	virtual void SetCustomEnabled(bool flag) override;


protected:

	InteractorStyleSurfaceCenterLineFindMaximumRadius();
	virtual ~InteractorStyleSurfaceCenterLineFindMaximumRadius() override;

	virtual void CreateCenterLine() override;
	virtual void InitializationSeeds();
	virtual void FindMaximumRadius();

	virtual void OnKeyPress() override;


	vtkSmartPointer<vtkKdTreePointLocator> m_pointLocator = nullptr;
	vtkSmartPointer<vtkSeedWidget> m_seedWidget = nullptr;
	vtkSmartPointer<vtkPolyData> m_triangulatedCenterLine = nullptr;
	vtkSmartPointer<vtkTextActor> m_radiusText = nullptr;
	//vtkSmartPointer<vtkPolyData> m_centerLine = nullptr;


private:

};



#endif // !__INTERACTOR_STYLE_SURFACE_CENTER_LINE_FIND_MAXIMUM_RADIUS_H__
