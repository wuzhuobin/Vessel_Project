#ifndef __IADE_INTERACTOR_STYLE_SWITCH3D_H__
#define __IADE_INTERACTOR_STYLE_SWITCH3D_H__

#include "InteractorStyleSwitch3D.h"


class IADEInteractorStyleSwitch3D: public InteractorStyleSwitch3D
{
public:
	static IADEInteractorStyleSwitch3D* New();
	vtkTypeMacro(IADEInteractorStyleSwitch3D, InteractorStyleSwitch3D);


protected:
	IADEInteractorStyleSwitch3D();

private:

};




#endif // !__IADE_INTERACTOR_STYLE_SWITCH3D_H__


