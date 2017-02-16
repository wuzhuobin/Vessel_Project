#ifndef __QINTERACTOR_STYLE_VBD_SMOKER_SEEDS_H__
#define __QINTERACTOR_STYLE_VBD_SMOKER_SEEDS_H__

#include "InteractorStyleSeedsPlacer.h"
#include "QAbstractNavigation.h"


namespace Ui { class QInteractorStyleVBDSmokerSeeds; }
class QInteractorStyleVBDSmokerSeeds : public QAbstractNavigation, 
	public InteractorStyleSeedsPlacer
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleVBDSmokerSeeds);

public:

	vtkTypeMacro(QInteractorStyleVBDSmokerSeeds, InteractorStyleSeedsPlacer);
	static QInteractorStyleVBDSmokerSeeds* New();
	void SetCustomEnabled(bool flag);
	void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

public slots:
	void SlotClearAllSeeds();
	void SetFocalSeed(int i);
	void DeleteFocalSeed();
	void SaveWidgetToSeeds();
	void DropSeed();

	void SlotVBDSmokerUpdate();
protected:
	QInteractorStyleVBDSmokerSeeds(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleVBDSmokerSeeds();
	void UpdateWidgetToSeeds(int* oldImagePos, int* newImagePos);


private:
	Ui::QInteractorStyleVBDSmokerSeeds* ui = nullptr;


};

#endif // !__QINTERACTOR_STYLE_VBD_SMOKER_SEEDS_H__
