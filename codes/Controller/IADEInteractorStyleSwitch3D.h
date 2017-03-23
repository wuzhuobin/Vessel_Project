#ifndef __IADE_INTERACTOR_STYLE_SWITCH3D_H__
#define __IADE_INTERACTOR_STYLE_SWITCH3D_H__

#include "InteractorStyleSwitch3D.h"
#include "InteractorStyleSurfaceCenterLineSimpleClipping.h"
#include "InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius.h"
#include "InteractorStyleSurfaceCenterLinePerpendicularMeasurement.h"
#include "InteractorStyleSurfaceCenterLineCurvedNavigation.h"
#include "InteractorStyleSurfaceCenterlineWaypoint.h"

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

	vtkGetMacro(CurvedNavigation, InteractorStyleSurfaceCenterLineCurvedNavigation*);
	SetInteractorStyle3DMacro(CurvedNavigation);
	CurrentStyle3DMacro(CurvedNavigation);

	vtkGetMacro(Waypoint, InteractorStyleSurfaceCenterlineWaypoint*);
	SetInteractorStyle3DMacro(Waypoint);
	CurrentStyle3DMacro(Waypoint);

	//virtual void SetCenterLine(vtkPolyData* centerline);

protected:
	IADEInteractorStyleSwitch3D();

	InteractorStyleSurfaceCenterLineSimpleClipping* CenterLine;
	InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius* FindMaximumRadius;
	InteractorStyleSurfaceCenterLinePerpendicularMeasurement* PerpendicularMeasurement;
	InteractorStyleSurfaceCenterLineCurvedNavigation* CurvedNavigation;
	InteractorStyleSurfaceCenterlineWaypoint* Waypoint;

private:

};




#endif // !__IADE_INTERACTOR_STYLE_SWITCH3D_H__


