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
class vtkWindowedSincPolyDataFilter;

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
	vtkGetObjectMacro(MarchingCubes, vtkDiscreteMarchingCubes);
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

	vtkDiscreteMarchingCubes* MarchingCubes;
	vtkWindowedSincPolyDataFilter* WindowedSincPolyDataFilter;
	vtkRenderWindow                 *RenderWindow;
	vtkRenderer                     *Renderer;
	vtkActor* SurfaceActor;
	vtkPolyDataMapper* SurfaceMapper;
	vtkRenderWindowInteractor       *Interactor;
	vtkInteractorStyle         *InteractorStyle;

	vtkLookupTable* LookupTable = nullptr;

	bool FirstRender;


	vtkAlgorithm* GetInputAlgorithm();
	vtkInformation* GetInputInformation();

private:

};




#endif // !__SURFACE_VIEWER_H__