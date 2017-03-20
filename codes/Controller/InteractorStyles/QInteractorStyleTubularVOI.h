#ifndef __QINTERACTOR_STYLE_TUBULAR_VOI_H__
#define __QINTERACTOR_STYLE_TUBULAR_VOI_H__

#include "QInteractorStyleLumenSeedsPlacer.h"

namespace Ui { class QInteractorStyleTubularVOI; }
class QInteractorStyleTubularVOI : public QInteractorStyleLumenSeedsPlacer
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleTubularVOI);

public:
	vtkTypeMacro(QInteractorStyleTubularVOI, QInteractorStyleLumenSeedsPlacer);
	static QInteractorStyleTubularVOI* New();


	virtual void ExtractSegmentation(QList<int*>& seed);
	virtual void ExtractVOI(QList<int*> &seed);
	//virtual void SetCustomEnabled(bool flag);
	//virtual void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);

	//virtual void SetFocalSeed(int i, QList<int*>& seeds);

	//virtual void GenerateWidgetFromSeeds(const QList<int*>& seeds);

	//virtual void SaveWidgetToSeeds(QList<int*>& seeds);
	//virtual void DropSeed(QList<int*>& seeds);

	virtual void UpdateWidgetToSeeds(
		QList<int*>& seeds,
		int* newImagePos,
		int* oldImagePos = nullptr);
	//virtual void ClearAllSeeds(QList<int*>& seeds);
	//virtual void DeleteFocalSeed(QList<int*>& seeds);

	//virtual void ExtractLumen(QList<int*>& seeds);
	//void ExtractSegmentation(QList<int*>& seed);

	/**
	* temporary fix
	*/
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
	virtual void SetExtractRadius(int radius);
	virtual void ExtractSegmentation();
	virtual void ExtractVOI();
	virtual void ResetVOI();

protected:
	QInteractorStyleTubularVOI(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleTubularVOI();

	void uniqueInitialization();
	void initialization();

	virtual void uniqueEnable() override;

	static QList<int*> m_tubularSeeds;
	virtual QListWidget* GetListWidget();

	int m_extractRadius = 10;
private:
	Ui::QInteractorStyleTubularVOI* ui = nullptr;
};

#endif // !__QINTERACTOR_STYLE_TUBULAR_VOI_H__