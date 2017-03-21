#include "ContourWidgetSeries.h"
#include "ContourWidgetSeriesRepresentation.h"

#include <vtkObjectFactory.h>
#include <vtkContourWidget.h>

#include <list>
#include <iterator>

vtkStandardNewMacro(ContourWidgetSeries);

class vtkContourList: public std::list<vtkContourWidget*>{};
typedef std::list<vtkContourWidget*>::iterator vtkContourListIterator;

void ContourWidgetSeries::DeleteContour(int i)
{
	if (this->Contours->size() <= static_cast< size_t >(i))
	{
		return;
	}

	ContourWidgetSeriesRepresentation *rep =
		static_cast<ContourWidgetSeriesRepresentation*>(this->WidgetRep);
	if (rep)
	{
		rep->RemoveHandle(i);
	}

	vtkContourList iter = this->Contours->begin();
	std::advance(iter, i);
	(*iter)->SetEnabled(0);
	(*iter)->RemoveObservers(vtkCommand::StartInteractionEvent);
	(*iter)->RemoveObservers(vtkCommand::InteractionEvent);
	(*iter)->RemoveObservers(vtkCommand::EndInteractionEvent);
	vtkContourWidget * w = (*iter);
	this->Contours->erase(iter);
	w->Delete();
}

vtkContourWidget * ContourWidgetSeries::GetContour(int i)
{
	if (this->Contours->size() <= static_cast< size_t >(i))
	{
		return NULL;
	}
	vtkContourListIterator iter = this->Contours->begin();
	std::advance(iter, i);
	return *iter;
}

ContourWidgetSeries::ContourWidgetSeries()
{
}

ContourWidgetSeries::~ContourWidgetSeries()
{
}

void ContourWidgetSeries::SetEnabled(int enabling)
{
	this->Superclass::SetEnabled(enabling);

	vtkContourListIterator iter;
	for (iter = this->Contours->begin(); iter != this->Contours->end(); ++iter)
	{
		(*iter)->SetEnabled(enabling);
	}

	if (!enabling)
	{
		this->RequestCursorShape(VTK_CURSOR_DEFAULT);
		this->WidgetState = vtkSeedWidget::Start;
	}

	this->Render();
}
