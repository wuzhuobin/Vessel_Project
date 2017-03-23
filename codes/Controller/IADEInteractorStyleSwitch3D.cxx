#include "IADEInteractorStyleSwitch3D.h"


#include <vtkObjectFactory.h>

vtkStandardNewMacro(IADEInteractorStyleSwitch3D);

IADEInteractorStyleSwitch3D::IADEInteractorStyleSwitch3D()
{

	this->CenterLine = InteractorStyleSurfaceCenterLineSimpleClipping::New();
	allStyles.push_back(this->CenterLine);

	this->FindMaximumRadius = InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::New();
	allStyles.push_back(this->FindMaximumRadius);

	this->PerpendicularMeasurement = InteractorStyleSurfaceCenterLinePerpendicularMeasurement::New();
	allStyles.push_back(this->PerpendicularMeasurement);

	this->CurvedNavigation = InteractorStyleSurfaceCenterLineCurvedNavigation::New();
	allStyles.push_back(this->CurvedNavigation);

	//this->Waypoint = InteractorStyleSurfaceCenterlineWaypoint::New();
	//allStyles.push_back(this->Waypoint);

	this->CurrentStyle = 0;

}
