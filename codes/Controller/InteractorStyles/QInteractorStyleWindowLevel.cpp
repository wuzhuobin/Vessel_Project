#include "QInteractorStyleWindowLevel.h"
#include "ui_qinteractorstylewindowlevel.h"
#include "ui_QAbstractNavigation.h"

#include <vtkObjectFactory.h>

vtkStandardNewMacro(QInteractorStyleWindowLevel);
QSETUP_UI_SRC(QInteractorStyleWindowLevel);

QInteractorStyleWindowLevel::QInteractorStyleWindowLevel(int uiType, QWidget * parent)
{
	QNEW_UI();
	QSpinBox* spinBoxWindowLevel[] = {
		ui->spinBoxWindowLevel0,
		ui->spinBoxWindowLevel1,
		ui->spinBoxWindowLevel2
	};
	QSpinBox* spinBoxWindowWidth[] = {
		ui->spinBoxWindowWidth0,
		ui->spinBoxWindowWidth1,
		ui->spinBoxWindowWidth2
	};
	m_spinBoxWindowLevel = spinBoxWindowLevel[numOfMyself - 1];
	m_spinBoxWindowWidth = spinBoxWindowWidth[numOfMyself - 1];
	connect(m_spinBoxWindowLevel, SIGNAL(valueChanged(int)),
		this, SLOT(SetLevel(int)), Qt::UniqueConnection);
	connect(m_spinBoxWindowWidth, SIGNAL(valueChanged(int)),
		this, SLOT(SetWindow(int)), Qt::UniqueConnection);
}

QInteractorStyleWindowLevel::~QInteractorStyleWindowLevel()
{
	QDELETE_UI();
}

void QInteractorStyleWindowLevel::uniqueCall()
{
	QAbstractNavigation::uniqueCall();
}

void QInteractorStyleWindowLevel::SetWindowByViewer(double window)
{
	InteractorStyleWindowLevel::SetWindowByViewer(window);
	//if (int(m_window + 0.5) != window) {
		m_spinBoxWindowWidth->setValue(window);
	//}
}

void QInteractorStyleWindowLevel::SetLevelByViewer(double level)
{
	InteractorStyleWindowLevel::SetLevelByViewer(level);
	//if (int(m_level + 0.5) != level) {
		m_spinBoxWindowLevel->setValue(level);
	//}
}

void QInteractorStyleWindowLevel::SetCustomEnabled(bool flag)
{
	InteractorStyleWindowLevel::SetCustomEnabled(flag);
	uniqueInvoke(flag);
}

void QInteractorStyleWindowLevel::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleWindowLevel::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStyleWindowLevel::SetWindow(int window)
{
	if (int(m_window + 0.5) != window) {
		InteractorStyleWindowLevel::SetWindow(window);
	}
}

void QInteractorStyleWindowLevel::SetLevel(int level)
{
	if (int(m_level + 0.5) != level) {
		InteractorStyleWindowLevel::SetLevel(level);
	}
}

