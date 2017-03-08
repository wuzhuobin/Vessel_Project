#include "IADEInteractorStyleSwitch3D.h"


#include <vtkObjectFactory.h>

vtkStandardNewMacro(IADEInteractorStyleSwitch3D);

void IADEInteractorStyleSwitch3D::SetCenterLine(vtkPolyData * centerline)
{
	for (std::list<vtkInteractorStyle*>::const_iterator cit = allStyles.cbegin();
		cit != allStyles.cend(); ++cit) {
		//AbstractSurfaceCenterLine* _style = dynamic_cast<AbstractSurfaceCenterLine*>(
		//	(*cit));
		AbstractSurfaceCenterLine* _style = AbstractSurfaceCenterLine::SafeDownCast(*cit);
		if (!_style) {
			_style->SetCenterLine(centerline);
		}
	}
}

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

	this->CurrentStyle = 0;

}
