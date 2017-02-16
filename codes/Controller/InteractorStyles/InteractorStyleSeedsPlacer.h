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
#include <vtkSeedWidget.h>
#include <vtkSeedRepresentation.h>

class SeedsPlacerWidget;
class SeedsPlacerRepresentation;


class InteractorStyleSeedsPlacer : public AbstractNavigation
{
public:                                                                                                                                                                             
	vtkTypeMacro(InteractorStyleSeedsPlacer, AbstractNavigation);
	static InteractorStyleSeedsPlacer* New();

	void SetCustomEnabled(bool flag);
	void SetFocalSeed(int i);
	void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

	virtual void ClearAllSeedWidget();
	virtual void GenerateWidgetFromSeeds();
	virtual void SaveWidgetToSeeds();
	virtual void DropSeed();

protected:
	InteractorStyleSeedsPlacer();
	~InteractorStyleSeedsPlacer();
	/**
	 * Supposed to only used in callback
	 */
	virtual void UpdateWidgetToSeeds(int* oldImagePos, int* newImagePos);

	virtual void OnMouseMove();
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnRightButtonDown();
	virtual void OnChar();
	virtual void OnKeyPress();
	virtual void OnLeave();



	/**
	* Using a static list to save all seeds and it will be shared by all other
	* InteractorStyleSeedsPlacer instances
	*/
	static std::list<int*> m_seeds;
	static int m_oldSeedsSize;
	static void ClearAllSeeds();

private:
	friend class SeedsPlacerWidget;
	vtkSmartPointer<SeedsPlacerRepresentation> m_seedRep;
	vtkSmartPointer<SeedsPlacerWidget> m_seedWidget;
	/**
	 * Invalide pick or any other error? CalculateIndex() will set m_world[3] = 
	 * { 0 };
	 */
	double m_world[3] = { 0 };

	void CalculateIndex();


};

#endif // !__INTERACTOR_STYLE_SEEDS_PLACER_H__
