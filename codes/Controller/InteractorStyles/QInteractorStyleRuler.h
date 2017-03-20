#ifndef __QInteractorStyleRuler_H__
#define __QInteractorStyleRuler_H__

#include "InteractorStyleRuler.h"
#include "QAbstractNavigation.h"

namespace Ui { class QInteractorStyleRuler; }


class QInteractorStyleRuler : public QAbstractNavigation,
	public InteractorStyleRuler
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleRuler);
public:
	vtkTypeMacro(QInteractorStyleRuler, InteractorStyleRuler);
	static QInteractorStyleRuler* New();
public slots:
	void SetCustomEnabled(bool flag);
	void SetCurrentFocalPointWithImageCoordinate(int i, int j, int k);


	void EnableMaximumWallThickneesLabel(bool flag);
protected:
	QInteractorStyleRuler(int uiType = UNIQUE_UI, QWidget* parent = Q_NULLPTR);
	~QInteractorStyleRuler ();

	void initialization();

private:


	Ui::QInteractorStyleRuler* ui = nullptr;
};

#endif // !__QInteractorStyleRuler_H__



