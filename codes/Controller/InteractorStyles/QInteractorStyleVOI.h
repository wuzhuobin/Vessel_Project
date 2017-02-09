#ifndef __QINTERACTOR_STYLE_VOI_H__
#define __QINTERACTOR_STYLE_VOI_H__

#include "QAbstractNavigation.h"
//#include "InteractorStyleNavigation.h"
#include "QInteractorStyleNavigation.h"

#include <vtkSmartPointer.h>

namespace Ui { class QInteractorStyleVOI; }
class vtkRenderWindow;
class vtkROIWidget;

class QInteractorStyleVOI :	
/*	public QAbstractNavigation,*/	public QInteractorStyleNavigation
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleVOI);
public:
	vtkTypeMacro(QInteractorStyleVOI, QInteractorStyleNavigation);
	static QInteractorStyleVOI* New();
	virtual void SetCustomEnabled(bool flag);


public slots:
	virtual void slotUpdateVOISpinBoxes(double* values);
	virtual void ExtractVOI();
	virtual void ResetVOI();

protected:
	QInteractorStyleVOI(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleVOI();


private:
	// initialization value is nullptr, if there is an instance of QInteractorStyleROI
	// m_roi will be created
	static vtkSmartPointer<vtkROIWidget> m_roi;
	static vtkSmartPointer<vtkRenderWindow> m_renderWindow;
	// for differentiate itself 
	int m_uniqueROIId = 0;
	Ui::QInteractorStyleVOI* ui = nullptr;
};

#endif // !__QINTERACTOR_STYLE_VOI_H__
