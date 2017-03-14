#ifndef __QINTERACTOR_STYLE_LUMEN_SEEDS_PLACER_H__
#define __QINTERACTOR_STYLE_LUMEN_SEEDS_PLACER_H__

#include "InteractorStyleSeedsPlacer.h"
#include "QAbstractNavigation.h"

class vtkImageData;
//class UpdateTargetViewerCallback;

namespace Ui { class QInteractorStyleLumenSeedsPlacer; }
class QInteractorStyleLumenSeedsPlacer: public QAbstractNavigation, 
	public InteractorStyleSeedsPlacer
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleLumenSeedsPlacer);

public:
	vtkTypeMacro(QInteractorStyleLumenSeedsPlacer, InteractorStyleSeedsPlacer);
	static QInteractorStyleLumenSeedsPlacer* New();
	virtual void SetCustomEnabled(bool flag);
	virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

	virtual void SetFocalSeed(int i, QList<int*>& seeds);

	virtual void GenerateWidgetFromSeeds(const QList<int*>& seeds);

	virtual void SaveWidgetToSeeds(QList<int*>& seeds);
	virtual void DropSeed(QList<int*>& seeds);

	virtual void UpdateWidgetToSeeds(
		QList<int*>& seeds,
		int* newImagePos,
		int* oldImagePos = nullptr);
	virtual void ClearAllSeeds(QList<int*>& seeds);
	virtual void DeleteFocalSeed(QList<int*>& seeds);

	virtual void ExtractLumen(QList<int*>& seeds);
	void ExtractSegmentation(QList<int*>& seed);

	/**
	 * temporary fix
	 */
	virtual void UpdateTargetViewer();
public slots:
	virtual void GenerateWidgetFromSeeds() override;
	virtual void SaveWidgetToSeeds() override;
	virtual void ClearAllSeeds() override;
	virtual void SetFocalSeed(int i) override;
	virtual void DeleteFocalSeed();
	virtual void DropSeed() override;
	virtual void UpdateWidgetToSeeds(
		int* newImagePos,
		int* oldImagePos = nullptr);

	virtual void ExtractLumen();
	void ExtractSegmentation();
	void SetExtractRadius(int radius);
	void SetMultipier(double value);
	void SetNumberOfIteractions(int value);
	void SetInitialNeighborhoodRadius(int value);

protected:
	QInteractorStyleLumenSeedsPlacer(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleLumenSeedsPlacer();

	void uniqueInitialization();
	void initialization();

	virtual void uniqueEnable() override;
	
	virtual void OnKeyPress() override;
	virtual void enterEvent(QEvent* event) override;

	static QList<int*> m_lumenSeeds;


private:
	Ui::QInteractorStyleLumenSeedsPlacer* ui = nullptr;

	int m_numberOfIteractions = 3;
	double m_multiplier = 2.1;
	int m_initialNeighborhoodRadius = 1;
	int m_extractRadius = 10;

	//friend class UpdateTargetViewerCallback;
	//UpdateTargetViewerCallback* callback = nullptr;

	//QList<vtkSmartPointer<vtkImageData>> m_listOfVtkImages;
	QStringList m_listOfModalityNames;

};




#endif // !__QINTERACTOR_STYLE_LUMEN_SEEDS_PLACER_H__
