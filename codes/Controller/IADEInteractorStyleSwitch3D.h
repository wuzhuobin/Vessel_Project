#ifndef __IADE_INTERACTOR_STYLE_SWITCH3D_H__
#define __IADE_INTERACTOR_STYLE_SWITCH3D_H__

#include "InteractorStyleSwitch3D.h"
#include "InteractorStyleSurfaceCenterLine.h"


class IADEInteractorStyleSwitch3D: public InteractorStyleSwitch3D
{
public:
	static IADEInteractorStyleSwitch3D* New();
	vtkTypeMacro(IADEInteractorStyleSwitch3D, InteractorStyleSwitch3D);
	vtkGetMacro(CenterLine, InteractorStyleSurfaceCenterLine*);
	SetInteractorStyle3DMacro(CenterLine);
	CurrentStyle3DMacro(CenterLine);

protected:
	IADEInteractorStyleSwitch3D();

	InteractorStyleSurfaceCenterLine* CenterLine;


private:

};




#endif // !__IADE_INTERACTOR_STYLE_SWITCH3D_H__


