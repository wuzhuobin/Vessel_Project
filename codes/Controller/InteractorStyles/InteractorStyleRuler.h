#ifndef __INTERACTORSTYLERULER_H
#define __INTERACTORSTYLERULER_H

#include "InteractorStyleNavigation.h"

#include <vtkSmartPointer.h>

class vtkDistanceWidget;
class vtkPointSetToLabelHierarchy;
class vtkStringArray;
class vtkLabelPlacementMapper;



class InteractorStyleRuler :
	public InteractorStyleNavigation
{
public:
	vtkTypeMacro(InteractorStyleRuler, InteractorStyleNavigation);
	static InteractorStyleRuler* New();
	virtual void SetCustomEnabled(bool flag);
	//void EnableMaximumWallThickneesLabel(bool flag);
	void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

protected:
	InteractorStyleRuler();
	~InteractorStyleRuler();

	//void UpdateMaximumWallThicknessLabel();

	//bool m_distanceWidgetEnabledFlag = false;
	//bool m_MaximumWallThickneesLabelFlag = false;
	//vtkPointHandleRepresentation2D* m_pointHandleRepresentation2D = nullptr;
	//vtkPointHandleRepresentation3D* m_pointHandleRepresentation3D = nullptrptr;
	//vtkDistanceRepresentation2D* m_distanceRepresentation2D = nullptr;
	vtkDistanceWidget* m_distanceWidget = nullptr;

	//std::list<InteractorStyleRuler*> rulers;

	// Maximun wall thickness stuff
	//vtkSmartPointer<vtkPolyData>					m_displayPD;
	//vtkSmartPointer<vtkActor>						m_lineActor;
	//vtkSmartPointer<vtkActor2D>						m_labelActor;
	//vtkSmartPointer<vtkStringArray>					m_labelArray;
	//vtkSmartPointer<vtkIntArray>					m_sizeArray;
	//vtkSmartPointer<vtkPointSetToLabelHierarchy>	m_p2labelfilter;

	//vtkSmartPointer<vtkLabelPlacementMapper> m_labelMapper;
	//vtkSmartPointer<vtkPolyDataMapper> m_mapper;
};

#endif // !__INTERACTORSTYLERULER_H



