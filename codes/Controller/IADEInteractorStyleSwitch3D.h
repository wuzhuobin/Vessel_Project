#ifndef __IADE_INTERACTOR_STYLE_SWITCH3D_H__
#define __IADE_INTERACTOR_STYLE_SWITCH3D_H__

#include "InteractorStyleSwitch3D.h"
#include "InteractorStyleSurfaceCenterLineSimpleClipping.h"
#include "InteractorStyleSurfaceCenterLineFindMaximumRadius.h"


class IADEInteractorStyleSwitch3D: public InteractorStyleSwitch3D
{
public:
	static IADEInteractorStyleSwitch3D* New();
	vtkTypeMacro(IADEInteractorStyleSwitch3D, InteractorStyleSwitch3D);
	vtkGetMacro(CenterLine, InteractorStyleSurfaceCenterLineSimpleClipping*);
	SetInteractorStyle3DMacro(CenterLine);
	CurrentStyle3DMacro(CenterLine);

	vtkGetMacro(FindMaximumRadius, InteractorStyleSurfaceCenterLineFindMaximumRadius*);
	SetInteractorStyle3DMacro(FindMaximumRadius);
	CurrentStyle3DMacro(FindMaximumRadius);

protected:
	IADEInteractorStyleSwitch3D();

	InteractorStyleSurfaceCenterLineSimpleClipping* CenterLine;
	InteractorStyleSurfaceCenterLineFindMaximumRadius* FindMaximumRadius;


private:

};




#endif // !__IADE_INTERACTOR_STYLE_SWITCH3D_H__


