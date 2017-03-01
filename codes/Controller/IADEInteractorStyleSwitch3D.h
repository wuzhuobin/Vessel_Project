#ifndef __IADE_INTERACTOR_STYLE_SWITCH3D_H__
#define __IADE_INTERACTOR_STYLE_SWITCH3D_H__

#include "InteractorStyleSwitch3D.h"
#include "InteractorStyleSurfaceCenterLineSimpleClipping.h"
#include "InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius.h"
#include "InteractorStyleSurfaceCenterLinePerpendicularMeasurement.h"


class IADEInteractorStyleSwitch3D: public InteractorStyleSwitch3D
{
public:
	static IADEInteractorStyleSwitch3D* New();
	vtkTypeMacro(IADEInteractorStyleSwitch3D, InteractorStyleSwitch3D);
	vtkGetMacro(CenterLine, InteractorStyleSurfaceCenterLineSimpleClipping*);
	SetInteractorStyle3DMacro(CenterLine);
	CurrentStyle3DMacro(CenterLine);

	vtkGetMacro(FindMaximumRadius, InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius*);
	SetInteractorStyle3DMacro(FindMaximumRadius);
	CurrentStyle3DMacro(FindMaximumRadius);

	vtkGetMacro(PerpendicularMeasurement, InteractorStyleSurfaceCenterLinePerpendicularMeasurement*);
	SetInteractorStyle3DMacro(PerpendicularMeasurement);
	CurrentStyle3DMacro(PerpendicularMeasurement);

protected:
	IADEInteractorStyleSwitch3D();

	InteractorStyleSurfaceCenterLineSimpleClipping* CenterLine;
	InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius* FindMaximumRadius;
	InteractorStyleSurfaceCenterLinePerpendicularMeasurement* PerpendicularMeasurement;

private:

};




#endif // !__IADE_INTERACTOR_STYLE_SWITCH3D_H__


