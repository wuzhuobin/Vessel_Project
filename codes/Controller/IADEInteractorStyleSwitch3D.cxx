#include "IADEInteractorStyleSwitch3D.h"


#include <vtkObjectFactory.h>

vtkStandardNewMacro(IADEInteractorStyleSwitch3D);

IADEInteractorStyleSwitch3D::IADEInteractorStyleSwitch3D()
{

	this->CenterLine = InteractorStyleSurfaceCenterLine::New();
	allStyles.push_back(this->CenterLine);

	this->CurrentStyle = 0;

}
