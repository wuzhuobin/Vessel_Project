#include "IADEInteractorStyleSwitch3D.h"


#include <vtkObjectFactory.h>

vtkStandardNewMacro(IADEInteractorStyleSwitch3D);

IADEInteractorStyleSwitch3D::IADEInteractorStyleSwitch3D()
{
	this->CurrentStyle = 0;

}
