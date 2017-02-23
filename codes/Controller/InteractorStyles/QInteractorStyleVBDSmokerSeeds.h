#ifndef __QINTERACTOR_STYLE_VBD_SMOKER_SEEDS_H__
#define __QINTERACTOR_STYLE_VBD_SMOKER_SEEDS_H__

#include "InteractorStyleSeedsPlacer.h"
#include "QAbstractNavigation.h"

#include <vtkSmartPointer.h>


namespace Ui { class QInteractorStyleVBDSmokerSeeds; }

class vtkHandleWidget;

class QInteractorStyleVBDSmokerSeeds : public QAbstractNavigation, 
	public InteractorStyleSeedsPlacer
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleVBDSmokerSeeds);

public:
	const static int NUM_OF_SPHERE_WIDGET = 11;

	vtkTypeMacro(QInteractorStyleVBDSmokerSeeds, InteractorStyleSeedsPlacer);
	static QInteractorStyleVBDSmokerSeeds* New();
	void SetCustomEnabled(bool flag);
	void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

	virtual void GenerateWidgetFromSeeds() override;
	virtual void SaveWidgetToSeeds() override;
	virtual void DropSeed() override;
	virtual void UpdateWidgetToSeeds(
		int* newImagePos,
		int* oldImagePos = nullptr) override;
	virtual void UpdateWidgetToSeeds(
		std::list<int*>& seeds,
		int* newImagePos,
		int* oldImagePos = nullptr) override;
	virtual void ClearAllSeeds() override;

	virtual void uniqueEnable() override;
	virtual void uniqueDisable() override;

protected slots:
	void slotBasilarArteryBifurcationLocationCurrentSlice();
	void slotPonsCentralSectionLocationCurrentSlice();

	void slotEnableTest(bool flag);


protected:
	QInteractorStyleVBDSmokerSeeds(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleVBDSmokerSeeds();

	static std::list<int*> m_VBDSmokerSeeds;

private:
	
	void uniqueInitialization();
	void initialization();
	
	Ui::QInteractorStyleVBDSmokerSeeds* ui = nullptr;

};

#endif // !__QINTERACTOR_STYLE_VBD_SMOKER_SEEDS_H__
