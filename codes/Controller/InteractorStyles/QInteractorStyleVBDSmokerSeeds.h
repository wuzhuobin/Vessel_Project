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
	const static int NUM_OF_ELONGATION = 4;
	const static int NUM_OF_DETOUR = 7;

	vtkTypeMacro(QInteractorStyleVBDSmokerSeeds, InteractorStyleSeedsPlacer);
	static QInteractorStyleVBDSmokerSeeds* New();
	void SetCustomEnabled(bool flag);
	void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

	virtual void GenerateWidgetFromSeeds() override;
	//virtual void GenerateWidgetFromSeeds(const std::list<int*>& seeds) override;
	/**
	 * save all widgets into the 2 list
	 * since all seeds has already created beforehand, no need to check and push
	 * juse replacing
	 */
	virtual void SaveWidgetToSeeds() override;
	/**
	 * @deprecated
	 * @oveerride
	 * do nothing
	 */
	virtual void UpdateWidgetToSeeds(
		int* newImagePos,
		int* oldImagePos = nullptr) override;
	/**
	* @deprecated
	* @oveerride
	* do nothing
	*/
	virtual void UpdateWidgetToSeeds(
		std::list<int*>& seeds,
		int* newImagePos,
		int* oldImagePos = nullptr) override;
	virtual void ClearAllSeeds() override;





protected slots:
	void slotBasilarArteryBifurcationLocationCurrentSlice();
	void slotPonsCentralSectionLocationCurrentSlice();

	void slotEnableTest(bool flag);
	virtual void DropSeed() override;


protected:
	QInteractorStyleVBDSmokerSeeds(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleVBDSmokerSeeds();

	virtual void uniqueEnable() override;
	virtual void uniqueDisable() override;

	static std::list<int*> m_VBDSmokerElongation;
	static std::list<int*> m_VBDSmokerDetour;

private:
	
	void uniqueInitialization();
	void initialization();
	
	Ui::QInteractorStyleVBDSmokerSeeds* ui = nullptr;

};

#endif // !__QINTERACTOR_STYLE_VBD_SMOKER_SEEDS_H__
