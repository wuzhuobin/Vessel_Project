#ifndef __INTERACTOR_STYLE_SURFACE_CENTER_LINE_H__
#define __INTERACTOR_STYLE_SURFACE_CENTER_LINE_H__

#include "AbstractSurfaceCenterLine.h"
#include <vtkSmartPointer.h>


class vtkPolyData;
class vtkIdList;

class InteractorStyleSurfaceCenterLineSimpleClipping: 
	public AbstractSurfaceCenterLine
{
public:

	enum
	{
		CENTER_LINE_ORIENTATION_YZ = 0,
		CENTER_LINE_ORIENTATION_XZ = 1,
		CENTER_LINE_ORIENTATION_XY = 2
	};
	static const char RADIUS[];


	vtkTypeMacro(InteractorStyleSurfaceCenterLineSimpleClipping, AbstractSurfaceCenterLine);
	static InteractorStyleSurfaceCenterLineSimpleClipping* New();

	virtual void SetCustomEnabled(bool flag) override;
	vtkSetMacro(m_centerLineOrientation, unsigned short);
	vtkGetMacro(m_centerLineOrientation, unsigned short);
	vtkSetMacro(m_ClipingDistance, int);
	vtkGetMacro(m_ClipingDistance, int);

protected:
	InteractorStyleSurfaceCenterLineSimpleClipping();
	virtual ~InteractorStyleSurfaceCenterLineSimpleClipping() override;

	virtual bool CreateCenterLine() override;
	/**
	 * if true, re clip
	 * false, change source id 
	 */
	virtual bool CreateCenterLine(bool reClipSurface);
	virtual void ClipAndCap();
	
	virtual void OnKeyPress();

	unsigned short m_centerLineOrientation = CENTER_LINE_ORIENTATION_XY;
	int m_ClipingDistance = 3;

	vtkSmartPointer<vtkIdList> m_capCenterIds = nullptr;
	vtkIdType m_sourceIdId = 0;
	vtkSmartPointer<vtkPolyData> m_ClipAndCapSurface = nullptr;


private:

};




#endif // !__INTERACTOR_STYLE_SURFACE_CENTER_LINE_H__