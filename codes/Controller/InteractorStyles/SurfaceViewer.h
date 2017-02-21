#ifndef __SURFACE_VIEWER_H__
#define __SURFACE_VIEWER_H__

#include <vtkObject.h>

class vtkImageData;
class vtkAlgorithmOutput;
class vtkAlgorithm;
class vtkInformation;
class vtkActor;
class vtkPolyDataMapper;
class vtkRenderWindow;
class vtkRenderer;
class vtkRenderWindowInteractor;
class vtkInteractorStyle;
class vtkDiscreteMarchingCubes;
class vtkLookupTable;
class vtkImageResample;
class vtkWindowedSincPolyDataFilter;
class vtkDepthSortPolyData;

class SurfaceViewer: public vtkObject
{
public:
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
	vtkGetObjectMacro(SurfaceActor, vtkActor);
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


	// Description:
	// Create a window in memory instead of on the screen. This may not
	// be supported for every type of window and on some windows you may
	// need to invoke this prior to the first render.
	virtual void SetOffScreenRendering(int flag);
	virtual int GetOffScreenRendering();
	vtkBooleanMacro(OffScreenRendering, int);


protected:

	SurfaceViewer();
	~SurfaceViewer();

	virtual void InstallPipeline();
	virtual void UnInstallPipeline();

	
	/**
	* Find out whether this box supports depth peeling. Depth peeling requires
	* a variety of openGL extensions and appropriate drivers.
	* rendered to screen (this requires the box to support off screen rendering)
	* @return	TRUE if depth peeling is supported, FALSE otherwise (which means
	*			that another strategy must be used for correct rendering of translucent
	*			geometry, e.g. CPU-based depth sorting)
	*/
	bool IsDepthPeelingSupported();

	/**
	* Setup the rendering environment for depth peeling (general depth peeling
	* support is requested).
	* @see IsDepthPeelingSupported()
	* @param renderWindow a valid openGL-supporting render window
	* @param renderer a valid renderer instance
	* @param maxNoOfPeels maximum number of depth peels (multi-pass rendering)
	* @param occulusionRation the occlusion ration (0.0 means a perfect image,
	* >0.0 means a non-perfect image which in general results in faster rendering)
	* @return TRUE if depth peeling could be set up
	*/
	bool SetupEnvironmentForDepthPeeling(int maxNoOfPeels = 0, double occlusionRatio = 0.0);


	//vtkImageMapToWindowLevelColors  *WindowLevel;

	vtkImageResample* ImageResample = nullptr;
	vtkDiscreteMarchingCubes* MarchingCubes = nullptr;
	vtkWindowedSincPolyDataFilter* WindowedSincPolyDataFilter = nullptr;
	vtkDepthSortPolyData* DepthSortPolyData = nullptr;
	vtkRenderWindow* RenderWindow = nullptr;
	vtkRenderer* Renderer = nullptr;
	vtkActor* SurfaceActor = nullptr;
	vtkPolyDataMapper* SurfaceMapper = nullptr;
	vtkRenderWindowInteractor* Interactor = nullptr;
	vtkInteractorStyle* InteractorStyle = nullptr;

	vtkLookupTable* LookupTable = nullptr;

	bool FirstRender = true;
	bool DepthPeelingSupportedFlag = true;
	bool UseDepthPeeling = true;
	bool UseDepthSorting = false;
	//int MaxNoOfPeels = 100;
	//double OcclusionRatio = 0.1;


	vtkAlgorithm* GetInputAlgorithm();
	vtkInformation* GetInputInformation();

private:

};




#endif // !__SURFACE_VIEWER_H__