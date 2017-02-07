#ifndef __SURFACE_VIEWER_H__
#define __SURFACE_VIEWER_H__

#include <vtkObject.h>

class vtkImageData;
class vtkAlgorithmOutput;
class vtkActor;
class vtkRenderWindow;
class vtkRenderer;
class vtkRenderWindowInteractor;
class vtkInteractorStyle;

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

protected:

	SurfaceViewer();
	~SurfaceViewer();

	virtual void InstallPipeline();
	virtual void UnInstallPipeline();


	//vtkImageMapToWindowLevelColors  *WindowLevel;
	vtkRenderWindow                 *RenderWindow;
	vtkRenderer                     *Renderer;
	vtkActor* SurfaceActor;
	vtkRenderWindowInteractor       *Interactor;
	vtkInteractorStyle         *InteractorStyle;

private:

};




#endif // !__SURFACE_VIEWER_H__