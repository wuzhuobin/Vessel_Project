#ifndef __QINTERACTOR_STYLE_LUMEN_SEEDS_PLACER_H__
#define __QINTERACTOR_STYLE_LUMEN_SEEDS_PLACER_H__

#include "InteractorStyleSeedsPlacer.h"
#include "QAbstractNavigation.h"

class vtkImageData;

namespace Ui { class QInteractorStyleLumenSeedsPlacer; }
class QInteractorStyleLumenSeedsPlacer: public QAbstractNavigation, 
	public InteractorStyleSeedsPlacer
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleLumenSeedsPlacer);

public:
	vtkTypeMacro(QInteractorStyleLumenSeedsPlacer, InteractorStyleSeedsPlacer);
	static QInteractorStyleLumenSeedsPlacer* New();
	void SetCustomEnabled(bool flag);
	void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);
	/**
	 * temporary fix
	 */
	void SetTargetImages(
		QList<vtkSmartPointer<vtkImageData>> listOfVtkImages, 
		QList<QString> listOfModalityNames);
	virtual void SaveWidgetToSeeds(std::list<int*>& seed = m_seeds)override;
	virtual void UpdateWidgetToSeeds(
		int* newImagePos,
		int* oldImagePos = nullptr);
	//virtual void UpdateWidgetToSeeds(
	//	std::list<int*>& seeds,
	//	int* newImagePos,
	//	int* oldImagePos = nullptr);
	//virtual void UpdateWidgetToSeeds(int* oldImagePos, int* newImagePos);

public slots:
	void SlotClearAllSeeds();
	virtual void SetFocalSeed(int i);
	void DeleteFocalSeed();
	void DropSeed();

	void ExtractLumen();
	/**
	 * @deprecated
	 * method has already been moved to InteractorStyleVesselSegmentation
	 */
	void ExtractLumenPolyData();
	void SetMultipier(double value);
	void SetNumberOfIteractions(int value);
	void SetInitialNeighborhoodRadius(int value);

protected:
	QInteractorStyleLumenSeedsPlacer(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleLumenSeedsPlacer();

	//void uniqueInvoke(bool flag);

	void OnKeyPress();

private:

	void uniqueInitialization();
	void initialization();

	Ui::QInteractorStyleLumenSeedsPlacer* ui = nullptr;

	int m_numberOfIteractions = 3;
	double m_multiplier = 2.1;
	int m_initialNeighborhoodRadius = 1;


	QList<vtkSmartPointer<vtkImageData>> m_listOfVtkImages;
	QList<QString> m_listOfModalityNames;

};




#endif // !__QINTERACTOR_STYLE_LUMEN_SEEDS_PLACER_H__
