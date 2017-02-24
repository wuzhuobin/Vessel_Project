/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


This class is an interactor modified from TrackBallCamera, providing an extra function
of switching slice planes position to the world position clicked on.


Wong Matthew Lun
Copyright (C) 2016
*/


#ifndef __INTERACTOR_STYLE_SEEDS_PLACER_H__
#define __INTERACTOR_STYLE_SEEDS_PLACER_H__

#include "AbstractNavigation.h"

#include <vtkSmartPointer.h>

class vtkSeedWidget;
class vtkSeedRepresentation;

class InteractorStyleSeedsPlacer : public AbstractNavigation
{
public:                                                                                                                                                                             
	vtkTypeMacro(InteractorStyleSeedsPlacer, AbstractNavigation);
	static InteractorStyleSeedsPlacer* New();

	void SetCustomEnabled(bool flag);
	virtual void SetFocalSeed(int i);
	virtual void SetFocalSeed(int i, std::list<int*>& seeds);
	void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);




	virtual void ClearAllSeedWidget();
	virtual void GenerateWidgetFromSeeds();
	virtual void GenerateWidgetFromSeeds(const std::list<int*>& seeds);
	virtual void SaveWidgetToSeeds();
	virtual void SaveWidgetToSeeds(std::list<int*>& seeds);
	virtual void DropSeed();
	virtual void DropSeed(std::list<int*>& seeds);
	virtual void UpdateWidgetToSeeds(
		int* newImagePos,
		int* oldImagePos = nullptr);
	virtual void UpdateWidgetToSeeds(
		std::list<int*>& seeds,
		int* newImagePos,
		int* oldImagePos = nullptr);
	virtual void ClearAllSeeds();
	virtual void ClearAllSeeds(std::list<int*>& seed);
	//virtual void DropSeed();

protected:
	InteractorStyleSeedsPlacer();
	~InteractorStyleSeedsPlacer();
	/**
	 * Supposed to only used in callback
	 */



	virtual void OnLeftButtonUp();
	virtual void OnKeyPress();

	/**
	* Using a static list to save all seeds and it will be shared by all other
	* InteractorStyleSeedsPlacer instances
	*/
	static std::list<int*> m_seeds;

	friend class SeedsPlacerWidget;
	vtkSmartPointer<vtkSeedRepresentation> m_seedRep = nullptr;
	vtkSmartPointer<vtkSeedWidget> m_seedWidget = nullptr;
	vtkSmartPointer<vtkCallbackCommand> m_callbackCommandSave = nullptr;

};

#endif // !__INTERACTOR_STYLE_SEEDS_PLACER_H__
