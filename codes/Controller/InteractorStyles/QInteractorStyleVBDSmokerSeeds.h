#ifndef __QINTERACTOR_STYLE_VBD_SMOKER_SEEDS_H__
#define __QINTERACTOR_STYLE_VBD_SMOKER_SEEDS_H__

#include "InteractorStyleNavigation.h"
#include "QAbstractNavigation.h"

#include <vtkSmartPointer.h>


namespace Ui { class QInteractorStyleVBDSmokerSeeds; }

class vtkHandleWidget;

class QInteractorStyleVBDSmokerSeeds : public QAbstractNavigation, 
	public InteractorStyleNavigation
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleVBDSmokerSeeds);

public:
	const static int NUM_OF_SPHERE_WIDGET = 11;

	vtkTypeMacro(QInteractorStyleVBDSmokerSeeds, InteractorStyleNavigation);
	static QInteractorStyleVBDSmokerSeeds* New();
	void SetCustomEnabled(bool flag);
	void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);


	virtual void uniqueEnable() override;
	virtual void uniqueDisable() override;

protected slots:
	void slotBasilarArteryBifurcationLocationCurrentSlice();
	void slotPonsCentralSectionLocationCurrentSlice();

	void slotEnableTest(bool flag);


protected:
	QInteractorStyleVBDSmokerSeeds(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleVBDSmokerSeeds();

	vtkSmartPointer<vtkHandleWidget> m_handleWidgets[NUM_OF_SPHERE_WIDGET] = {nullptr};

private:
	
	void uniqueInitialization();
	void initialization();
	
	Ui::QInteractorStyleVBDSmokerSeeds* ui = nullptr;

};

#endif // !__QINTERACTOR_STYLE_VBD_SMOKER_SEEDS_H__
