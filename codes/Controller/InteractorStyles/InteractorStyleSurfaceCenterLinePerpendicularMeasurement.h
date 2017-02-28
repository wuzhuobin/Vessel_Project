#ifndef __INTERACTOR_STYLE_SURFAEC_CENTER_LINE_PERPENDICULAT_MEASUREMENT_H__
#define __INTERACTOR_STYLE_SURFAEC_CENTER_LINE_PERPENDICULAT_MEASUREMENT_H__

#include "InteractorStyleSurfaceCenterLineSimpleClipping.h"

class vtkLineWidget2;
class vtkTextActor;

class InteractorStyleSurfaceCenterLinePerpendicularMeasurement :
	public InteractorStyleSurfaceCenterLineSimpleClipping
{
public:

	//const static unsigned int NUM_OF_HANDLES = 2;

	vtkTypeMacro(InteractorStyleSurfaceCenterLinePerpendicularMeasurement,
		InteractorStyleSurfaceCenterLineSimpleClipping);
	static InteractorStyleSurfaceCenterLinePerpendicularMeasurement* New();

	virtual void SetCustomEnabled(bool flag) override;


protected:

	InteractorStyleSurfaceCenterLinePerpendicularMeasurement();
	virtual ~InteractorStyleSurfaceCenterLinePerpendicularMeasurement() override;

	virtual void InitializeLineWidget2();

	virtual void OnKeyPress() override;

	vtkSmartPointer<vtkLineWidget2> m_lineWidget2 = nullptr;
	vtkSmartPointer<vtkTextActor> m_radiusText = nullptr;


private:

};



#endif // !__INTERACTOR_STYLE_SURFAEC_CENTER_LINE_PERPENDICULAT_MEASUREMENT_H__
