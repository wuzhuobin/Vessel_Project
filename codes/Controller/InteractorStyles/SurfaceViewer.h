#ifndef __SURFACE_VIEWER_H__
#define __SURFACE_VIEWER_H__

#include <vtkObject.h>
#include <vector>

class vtkImageData;
class vtkAlgorithmOutput;
class vtkAlgorithm;
class vtkInformation;
class vtkActor;
//class vtkPolyDataMapper;
class vtkRenderWindow;
class vtkRenderer;
class vtkRenderWindowInteractor;
class vtkInteractorStyle;
class vtkDiscreteMarchingCubes;
class vtkLookupTable;
class vtkImageResample;
class vtkWindowedSincPolyDataFilter;
class vtkActorCollection;
class vtkThreshold;
class vtkGeometryFilter;

class SurfaceViewer: public vtkObject
{
public:
	const static unsigned int INITIAL_ACTORS = 10;

	static SurfaceViewer* New();
	vtkTypeMacro(SurfaceViewer, vtkObject);
	void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// Get the name of rendering window.
	virtual const char *GetWindowName();

	// Description:
	// Render the resulting image.
	virtual void Render(void);

	// Description:
	// Set/Get the input image to the viewer.
	virtual void SetInputData(vtkImageData *in);
	virtual vtkImageData *GetInput();
	virtual void SetInputConnection(vtkAlgorithmOutput* input);

	virtual void UpdateDisplayExtent();

	virtual void SetLookupTable(vtkLookupTable* lookupTable);
	virtual vtkLookupTable* GetLookupTable();

	// Description:
	// Get the internal render window, renderer, image actor, and
	// image map instances.
	vtkGetObjectMacro(RenderWindow, vtkRenderWindow);
	vtkGetObjectMacro(Renderer, vtkRenderer);
	vtkGetObjectMacro(SurfaceActors, vtkActorCollection);
	vtkGetObjectMacro(ImageResample, vtkImageResample);
	vtkGetObjectMacro(MarchingCubes, vtkDiscreteMarchingCubes);
	vtkGetObjectMacro(WindowedSincPolyDataFilter, vtkWindowedSincPolyDataFilter)
	vtkGetObjectMacro(InteractorStyle, vtkInteractorStyle);

	// Description:
	// Set your own renderwindow and renderer
	virtual void SetRenderWindow(vtkRenderWindow *arg);
	virtual void SetRenderer(vtkRenderer *arg);

	// Description:
	// Attach an interactor for the internal render window.
	virtual void SetupInteractor(vtkRenderWindowInteractor* arg);

protected:

	SurfaceViewer();
	~SurfaceViewer();

	virtual void InstallPipeline();
	virtual void UnInstallPipeline();


	//vtkImageMapToWindowLevelColors  *WindowLevel;

	vtkImageResample* ImageResample;
	vtkDiscreteMarchingCubes* MarchingCubes;
	vtkWindowedSincPolyDataFilter* WindowedSincPolyDataFilter;

	std::vector<vtkGeometryFilter>* GeometryFilters;
	std::vector<vtkThreshold>* Thresholds;

	vtkRenderWindow                 *RenderWindow;
	vtkRenderer                     *Renderer;
	vtkActorCollection* SurfaceActors;
	//vtkPolyDataMapper* SurfaceMapper;
	vtkRenderWindowInteractor       *Interactor;
	vtkInteractorStyle         *InteractorStyle;

	vtkLookupTable* LookupTable = nullptr;

	bool FirstRender;


	vtkAlgorithm* GetInputAlgorithm();
	vtkInformation* GetInputInformation();

private:

};




#endif // !__SURFACE_VIEWER_H__