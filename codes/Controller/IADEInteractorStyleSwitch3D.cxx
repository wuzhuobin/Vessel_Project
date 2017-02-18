#include "IADEInteractorStyleSwitch3D.h"


#include <vtkObjectFactory.h>

vtkStandardNewMacro(IADEInteractorStyleSwitch3D);

IADEInteractorStyleSwitch3D::IADEInteractorStyleSwitch3D()
{

	this->CenterLine = InteractorStyleSurfaceCenterLineSimpleClipping::New();
	allStyles.push_back(this->CenterLine);

	this->FindMaximumRadius = InteractorStyleSurfaceCenterLineFindMaximumRadius::New();
	allStyles.push_back(this->FindMaximumRadius);

	this->CurrentStyle = 0;

}
