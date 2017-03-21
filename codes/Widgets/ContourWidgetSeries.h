#ifndef __CONTOUR_WIDGET_SERIES_H__
#define __CONTOUR_WIDGET_SERIES_H__

#include <vtkAbstractWidget.h>

class vtkContourRepresentation;
class vtkContourWidget;
class vtkContourList;
class ContourWidgetSeriesRepresentation;


class ContourWidgetSeries: public vtkAbstractWidget
{
public:
	// Description:
	// Instantiate this class.
	static ContourWidgetSeries *New();

	// Description:
	// Standard methods for a VTK class.
	vtkTypeMacro(ContourWidgetSeries, vtkAbstractWidget);
	void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// The method for activating and deactivating this widget. This method
	// must be overridden because it is a composite widget and does more than
	// its superclasses' vtkAbstractWidget::SetEnabled() method.
	virtual void SetEnabled(int enabling);

	// Description:
	// Set the current renderer. This method also propagates to all the child
	// handle widgets, if any exist
	virtual void SetCurrentRenderer(vtkRenderer *);

	// Description:
	// Set the interactor. This method also propagates to all the child
	// handle widgets, if any exist
	virtual void SetInteractor(vtkRenderWindowInteractor *);

	// Description:
	// Specify an instance of vtkWidgetRepresentation used to represent this
	// widget in the scene. Note that the representation is a subclass of vtkProp
	// so it can be added to the renderer independent of the widget.
	void SetRepresentation(ContourWidgetSeriesRepresentation *rep)
	{
		this->Superclass::SetWidgetRepresentation(
			reinterpret_cast<vtkWidgetRepresentation*>(rep));
	}

	// Description:
	// Return the representation as a vtkSeedRepresentation.
	ContourWidgetSeriesRepresentation *GetSeedRepresentation()
	{
		return reinterpret_cast<ContourWidgetSeriesRepresentation*>(this->WidgetRep);
	}

	// Description:
	// Create the default widget representation if one is not set.
	void CreateDefaultRepresentation();

	// Description:
	// Methods to change the whether the widget responds to interaction.
	// Overridden to pass the state to component widgets.
	virtual void SetProcessEvents(int);

	// Description:
	// Method to be called when the seed widget should stop responding to
	// the place point interaction. The seed widget, when defined allows you
	// place seeds by clicking on the render window. Use this method to
	// indicate that you would like to stop placing seeds interactively. If
	// you'd like the widget to stop responding to *any* user interaction
	// simply disable event processing by the widget by calling
	//   widget->ProcessEventsOff()
	virtual void CompleteInteraction();

	// Description:
	// Method to be called when the seed widget should start responding
	// to the interaction.
	virtual void RestartInteraction();

	// Description:
	// Use this method to programmatically create a new handle. In interactive
	// mode, (when the widget is in the PlacingSeeds state) this method is
	// automatically invoked. The method returns the handle created.
	// A valid seed representation must exist for the widget to create a new
	// handle.
	virtual vtkContourWidget * CreateNewContourWidget();

	// Description:
	// Delete the nth seed.
	void DeleteContour(int i);

	// Description:
	// Get the nth seed
	vtkContourWidget * GetContour(int i);

	// Description:
	// Get the widget state.
	vtkGetMacro(WidgetState, int);

	// The state of the widget
	//BTX
	enum
	{
		Start = 1,
		PlacingSeeds = 2,
		PlacedSeeds = 4,
		MovingSeed = 8
	};
	//ETX

protected:
	ContourWidgetSeries();
	~ContourWidgetSeries();


	int WidgetState;

	// Callback interface to capture events when
	// placing the widget.
	static void AddContourAction(vtkAbstractWidget*);
	static void CompletedAction(vtkAbstractWidget*);
	static void MoveAction(vtkAbstractWidget*);
	static void EndSelectAction(vtkAbstractWidget*);
	static void DeleteAction(vtkAbstractWidget*);

	// The positioning handle widgets
	vtkContourList *Contours;

	// Manipulating or defining ?
	int Defining;

private:
	ContourWidgetSeries(const ContourWidgetSeries&);  //Not implemented
	void operator=(const ContourWidgetSeries&);  //Not implemented

};





#endif // !__CONTOUR_WIDGET_SERIES_H__
