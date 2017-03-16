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
class vtkAxesActor;

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
	vtkGetObjectMacro(WindowedSincPolyDataFilter, vtkWindowedSincPolyDataFilter);
	vtkGetObjectMacro(DepthSortPolyData, vtkDepthSortPolyData);
	vtkGetObjectMacro(InteractorStyle, vtkInteractorStyle);
	vtkGetObjectMacro(AxesActor, vtkAxesActor);
	// Description:
	// Set your own renderwindow and renderer
	virtual void SetRenderWindow(vtkRenderWindow *arg);
	virtual void SetRenderer(vtkRenderer *arg);

	// Description:
	// Attach an interactor for the internal render window.
	virtual void SetupInteractor(vtkRenderWindowInteractor* arg);

	vtkBooleanMacro(EnableDepthSorting, bool);
	virtual void SetEnableDepthSorting(bool flag);


	vtkBooleanMacro(EnableDepthPeeling, bool);
	virtual void SetEnableDepthPeeling(bool flag);


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
	vtkAxesActor* AxesActor = nullptr;

	vtkLookupTable* LookupTable = nullptr;

	bool FirstRender = true;
	// MaxNoOfPeels maximum number of depth peels(multi - pass rendering)
	int MaxNoOfPeels = 0;
	// OcclusionRatio the occlusion ration(0.0 means a perfect image,
	// >0.0 means a non - perfect image which in general results in faster rendering)
	double OcclusionRatio = 0.0;


	vtkAlgorithm* GetInputAlgorithm();
	vtkInformation* GetInputInformation();

private:
	void SetInputConnection(vtkAlgorithmOutput* input); // not implemented

};




#endif // !__SURFACE_VIEWER_H__