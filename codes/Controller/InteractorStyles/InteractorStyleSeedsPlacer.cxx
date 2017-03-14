/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


This class is an interactor updatedOverlay from TrackBallCamera, providing an extra function
of switching slice planes position to the world position clicked on.


Wong Matthew Lun
Copyright (C) 2016
*/
#include "InteractorStyleSeedsPlacer.h"

#include <vtkRenderWindowInteractor.h>
#include <vtkAbstractPicker.h>
#include <vtkProperty.h>
#include <vtkCallbackCommand.h>
#include <vtkObjectFactory.h>
#include <vtkImageActorPointPlacer.h>
#include <vtkPointHandleRepresentation3D.h>
#include <ImageViewer.h>
#include <vtkWidgetEvent.h>
#include <vtkWidgetCallbackMapper.h>
#include <vtkHandleWidget.h>
#include <vtkSeedWidget.h>
#include <vtkSeedRepresentation.h>


#include <algorithm>



using namespace std;

class SeedsPlacerRepresentation : public vtkSeedRepresentation
{
public:
	vtkTypeMacro(SeedsPlacerRepresentation, vtkSeedRepresentation);
	static SeedsPlacerRepresentation* New();

	vtkImageActorPointPlacer* GetImagePointPlacer();
protected:
	SeedsPlacerRepresentation();

};

class SeedsPlacerWidget : public vtkSeedWidget
{
public:
	vtkTypeMacro(SeedsPlacerWidget, vtkSeedWidget);
	static SeedsPlacerWidget* New();
protected:
	SeedsPlacerWidget();
	//static void MoveAction(vtkAbstractWidget *w){
	//	w->InvokeEvent(vtkCommand::LeftButtonPressEvent);
	//}
	//static void DeleteAction(vtkAbstractWidget *w) {}

private:
	friend class InteractorStyleSeedsPlacer;
	InteractorStyleSeedsPlacer* seedsPlacer;
	static void CallbackSave(vtkObject* caller,
		unsigned long eid, void* clientdata, void *calldata);
	static int oldImagePos[3];
};

vtkStandardNewMacro(SeedsPlacerRepresentation);
vtkStandardNewMacro(SeedsPlacerWidget);
vtkStandardNewMacro(InteractorStyleSeedsPlacer);

//int InteractorStyleSeedsPlacer::m_oldSeedsSize = 0;
vector<int*> InteractorStyleSeedsPlacer::m_seeds;

void InteractorStyleSeedsPlacer::SetCustomEnabled(bool flag)
{
	if (flag) {
		SeedsPlacerRepresentation::SafeDownCast(m_seedRep)->GetImagePointPlacer()->SetImageActor(GetImageViewer()->GetImageActor());
		// for some opengl error
		//SeedsPlacerRepresentation::SafeDownCast(m_seedRep)->GetImagePointPlacer()->SetPixelTolerance(2);
		// SetInteractor CANNOT place in the constructor
		m_seedWidget->SetInteractor(this->Interactor);
		m_seedWidget->EnabledOn();
		m_seedWidget->On();
		GenerateWidgetFromSeeds();
	}
	else {
		m_seedWidget->EnabledOff();
		m_seedWidget->Off();
		ClearAllSeedWidget();
	}
	m_customFlag = flag;
}

void InteractorStyleSeedsPlacer::SetFocalSeed(int i, vector<int*>& seeds)
{
	//if (i >= m_seedRep->GetNumberOfSeeds() || i == m_focalSeed) {
	//	return;
	//}
	//m_focalSeed = i;
	if (i < 0 || i > seeds.size()) {
		return;
	}
	int imageCoordinate[3];
	memcpy(imageCoordinate, m_seeds[i], sizeof(imageCoordinate));
	SetCurrentFocalPointWithImageCoordinate(imageCoordinate[0], imageCoordinate[1],
		imageCoordinate[2]);
}

void InteractorStyleSeedsPlacer::SetFocalSeed(int i)
{
	SetFocalSeed(i, m_seeds);
}

void InteractorStyleSeedsPlacer::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	// Only Generate seedWidgets when it is needed
	// if its slice has not changed, it needs not to re-generate
	int oldIJK[3];
	GetImageViewer()->GetFocalPointWithImageCoordinate(oldIJK);
	AbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	for (list<AbstractInteractorStyle*>::const_iterator cit =
		m_abstractInteractorStyles.cbegin(); cit != m_abstractInteractorStyles.cend(); ++cit) {
		InteractorStyleSeedsPlacer* _style = dynamic_cast<InteractorStyleSeedsPlacer*>(*cit);
		if (_style != nullptr &&
			_style->m_customFlag &&
			_style->GetSlice() != oldIJK[_style->GetSliceOrientation()]) {
			_style->GenerateWidgetFromSeeds();
			_style->m_seedWidget->Render();
		}
	}
	//STYLE_DOWN_CAST_CONSTITERATOR(InteractorStyleSeedsPlacer, GenerateWidgetFromSeeds());
	//MY_VIEWER_CONSTITERATOR(Render());
}

InteractorStyleSeedsPlacer::InteractorStyleSeedsPlacer()
	:AbstractNavigation()
{
	m_seedWidget = vtkSmartPointer<SeedsPlacerWidget>::New();
	SeedsPlacerWidget::SafeDownCast(m_seedWidget)->seedsPlacer = this;
	m_seedRep = vtkSmartPointer<SeedsPlacerRepresentation>::New();
	m_seedWidget->SetRepresentation(m_seedRep);
}

InteractorStyleSeedsPlacer::~InteractorStyleSeedsPlacer()
{
	this->m_seedWidget->EnabledOff();
	this->m_seedWidget->SetInteractor(NULL);
	ClearAllSeeds();
}

void InteractorStyleSeedsPlacer::OnLeftButtonUp()
{
	AbstractNavigation::OnLeftButtonUp();
	
	//SaveWidgetToSeeds();
	// update other viewers' seed widgets
	for (list<AbstractInteractorStyle*>::const_iterator cit =
		m_abstractInteractorStyles.cbegin(); cit != m_abstractInteractorStyles.cend(); ++cit) {
		InteractorStyleSeedsPlacer* _style = dynamic_cast<InteractorStyleSeedsPlacer*>(*cit);
		if (_style != nullptr && _style->m_customFlag && _style != this ) {
			_style->GenerateWidgetFromSeeds();
		}
	}
}

void InteractorStyleSeedsPlacer::OnKeyPress()
{
	string key = this->Interactor->GetKeySym();
	cout << __func__ << ' ' << key << endl;
	if (key == "D") {

	}
	else {
		AbstractNavigation::OnKeyPress();
	}
}



//void InteractorStyleSeedsPlacer::GenerateWidgetFromSeeds()
//{
//	GenerateWidgetFromSeeds(m_seeds);
//}

void InteractorStyleSeedsPlacer::GenerateWidgetFromSeeds(const vector<int*>& seeds)
{
	ClearAllSeedWidget();


	for (vector<int*>::const_iterator cit = seeds.cbegin();
		cit != seeds.cend(); ++cit) {
		int* imagePos = (*cit);
		double worldPos[3];
		for (int pos = 0; pos < 3; ++pos) {
			worldPos[pos] = (imagePos[pos] * GetSpacing()[pos]) + GetOrigin()[pos];
		}
		if (imagePos[GetSliceOrientation()] == GetImageViewer()->GetSlice()) {
			vtkHandleWidget* newSeed = m_seedWidget->CreateNewHandle();
			newSeed->GetHandleRepresentation()->SetWorldPosition(worldPos);
			newSeed->EnabledOn();
		}
	}
	m_seedWidget->Render();
}

void InteractorStyleSeedsPlacer::SaveWidgetToSeeds()
{
	SaveWidgetToSeeds(m_seeds);
}

void InteractorStyleSeedsPlacer::UpdateWidgetToSeeds(int* newImagePos, int* oldImagePos)
{
	UpdateWidgetToSeeds(m_seeds, newImagePos, oldImagePos);
}

void InteractorStyleSeedsPlacer::UpdateWidgetToSeeds(vector<int*>& seeds, int * newImagePos, int * oldImagePos)
{

	// if oldImagePos is nullptr or oldImagePos is not a valid number, 
	// replace the existed newImagePos or push the new newImagePos
	if (oldImagePos == nullptr || 
		oldImagePos[0] < 0 || 
		oldImagePos[1] < 0 || 
		oldImagePos[2] < 0) {
		oldImagePos = newImagePos;
	}
	vector<int*>::iterator it = find_if(seeds.begin(),
		seeds.end(), [oldImagePos](int* index)->bool {
		return
			oldImagePos[0] == index[0] &&
			oldImagePos[1] == index[1] &&
			oldImagePos[2] == index[2];
	});
	if (it != seeds.end()) {
		memcpy(*it, newImagePos, sizeof(int[3]));
	}
	else {
		seeds.push_back(new int[3]);
		memcpy(seeds.back(), newImagePos, sizeof(int[3]));
	}
}

void InteractorStyleSeedsPlacer::ClearAllSeeds()
{
	ClearAllSeeds(m_seeds);
}

void InteractorStyleSeedsPlacer::SaveWidgetToSeeds(vector<int*>& seeds)
{
	for (int i = m_seedRep->GetNumberOfSeeds() - 1; i >= 0; --i) {
		//double* worldPos = new double[3]; // #MemLeakHere
		double worldPos[3];
		m_seedWidget->GetSeedRepresentation()->GetSeedWorldPosition(i, worldPos);
		
		int imagePos[3];
		for (int pos = 0; pos < 3; ++pos) {
			imagePos[pos] = (worldPos[pos] - GetOrigin()[pos]) / GetSpacing()[pos] + 0.5;
		}
		UpdateWidgetToSeeds(seeds, imagePos);
	}
}

void InteractorStyleSeedsPlacer::DropSeed()
{
	DropSeed(m_seeds);
}

void InteractorStyleSeedsPlacer::DropSeed(vector<int*>& seeds)
{
	double* worldPos = GetImageViewer()->GetFocalPointWithWorldCoordinate();
	vtkHandleWidget* newSeed = m_seedWidget->CreateNewHandle();
	newSeed->GetHandleRepresentation()->SetWorldPosition(worldPos);
	newSeed->EnabledOn();
	m_seedWidget->Render();

	SaveWidgetToSeeds(seeds);
	OnLeftButtonUp();
}

void InteractorStyleSeedsPlacer::ClearAllSeedWidget()
{
	for (int i = m_seedRep->GetNumberOfSeeds() - 1; i >= 0; --i) {
		m_seedWidget->DeleteSeed(i);
	}
}

void InteractorStyleSeedsPlacer::GenerateWidgetFromSeeds()
{
	GenerateWidgetFromSeeds(m_seeds);
}

void InteractorStyleSeedsPlacer::ClearAllSeeds(vector<int*>& seed)
{
	if (seed.size() != 0) {
		while (!seed.empty())
		{
			delete[] seed.back();
			seed.pop_back();
		}
	}
}
	
SeedsPlacerWidget::SeedsPlacerWidget()
{
	//this->CallbackMapper->SetCallbackMethod(vtkCommand::MouseMoveEvent,
	//	vtkWidgetEvent::Move,
	//	this, SeedsPlacerWidget::MoveAction);
	//this->CallbackMapper->SetCallbackMethod(vtkCommand::KeyPressEvent,
	//	vtkEvent::NoModifier, 127, 1, "Delete",
	//	vtkWidgetEvent::Delete,
	//	this, SeedsPlacerWidget::DeleteAction);

	// using a NULL function to replace CompletedAction(vtkAbstractWidget *w) {};
	this->CallbackMapper->SetCallbackMethod(vtkCommand::RightButtonPressEvent,
		vtkWidgetEvent::Completed,
		this, [](vtkAbstractWidget* widget) {});
	vtkSmartPointer<vtkCallbackCommand> callback =
		vtkSmartPointer<vtkCallbackCommand>::New();
	callback->SetCallback(SeedsPlacerWidget::CallbackSave);
	this->AddObserver(vtkCommand::StartInteractionEvent, callback);
	this->AddObserver(vtkCommand::EndInteractionEvent, callback);
	this->AddObserver(vtkCommand::PlacePointEvent, callback);
}

int SeedsPlacerWidget::oldImagePos[3] = {-1};

void SeedsPlacerWidget::CallbackSave(vtkObject * caller, unsigned long eid, void * clientdata, void * calldata)
{
	SeedsPlacerWidget* _widget = static_cast<SeedsPlacerWidget*>(caller);
	vtkSeedRepresentation* seedRep = _widget->GetSeedRepresentation();
	double worldPos[3];
	int seedNum = seedRep->GetActiveHandle();
	int newImagePos[3];
	seedRep->GetSeedWorldPosition(seedNum, worldPos);
	if (eid == vtkCommand::EndInteractionEvent ) {

		for (int pos = 0; pos < 3; ++pos) {
			newImagePos[pos] = (worldPos[pos] - _widget->seedsPlacer->GetOrigin()[pos]) /
				_widget->seedsPlacer->GetSpacing()[pos] + 0.5;
		}
		_widget->seedsPlacer->UpdateWidgetToSeeds(newImagePos, oldImagePos);
		_widget->seedsPlacer->OnLeftButtonUp();
	}
	else if (eid == vtkCommand::PlacePointEvent) {
		_widget->seedsPlacer->SaveWidgetToSeeds();
	}
	else if (eid == vtkCommand::StartInteractionEvent) {
		for (int pos = 0; pos < 3; ++pos) {
			oldImagePos[pos] = (worldPos[pos] - _widget->seedsPlacer->GetOrigin()[pos]) /
				_widget->seedsPlacer->GetSpacing()[pos] + 0.5;
		}
	}
}

vtkImageActorPointPlacer * SeedsPlacerRepresentation::GetImagePointPlacer()
{
	return vtkImageActorPointPlacer::SafeDownCast(
		GetHandleRepresentation()->
		GetPointPlacer());
}

SeedsPlacerRepresentation::SeedsPlacerRepresentation()
{
	SetHandleRepresentation(
		vtkSmartPointer<vtkPointHandleRepresentation3D>::New());
	GetHandleRepresentation()->SetPointPlacer(
		vtkSmartPointer<vtkImageActorPointPlacer>::New());
}