#include "QInteractorStyleWindowLevel.h"
#include "ui_qinteractorstylewindowlevel.h"
#include "ui_QAbstractNavigation.h"
#include "ImageViewer.h"

#include <vtkImageData.h>
#include <vtkObjectFactory.h>

#include <qpushbutton.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qlabel.h>

vtkStandardNewMacro(QInteractorStyleWindowLevel);
QSETUP_UI_SRC(QInteractorStyleWindowLevel);

QInteractorStyleWindowLevel::QInteractorStyleWindowLevel(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleWindowLevel::~QInteractorStyleWindowLevel()
{
	QDELETE_UI();
}

void QInteractorStyleWindowLevel::uniqueInvoke(bool flag)
{
	QAbstractNavigation::uniqueInvoke(flag);
	SetWindowByViewer(GetWindow());
	SetLevelByViewer(GetLevel());
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

void QInteractorStyleWindowLevel::initialization()
{
	m_label = new QLabel(this);
	m_spinBoxWindowLevel = new QSpinBox(this);
	m_spinBoxWindowWidth = new QSpinBox(this);
	m_sliderWindowLevel = new QSlider(Qt::Horizontal, this);
	m_sliderWindowWidth = new QSlider(Qt::Horizontal, this);
	m_pushButtonReset = new QPushButton("Reset", this);


	ui->gridLayout->addWidget(m_label, numOfMyself, 0);
	ui->gridLayout->addWidget(m_spinBoxWindowWidth, numOfMyself, 1);
	ui->gridLayout->addWidget(m_sliderWindowWidth, numOfMyself, 2);
	ui->gridLayout->addWidget(m_spinBoxWindowLevel, numOfMyself, 3);
	ui->gridLayout->addWidget(m_sliderWindowLevel, numOfMyself, 4);
	ui->gridLayout->addWidget(m_pushButtonReset, numOfMyself, 5);


	//QSpinBox* spinBoxWindowLevel[] = {
	//	ui->spinBoxWindowLevel0,
	//	ui->spinBoxWindowLevel1,
	//	ui->spinBoxWindowLevel2
	//};
	//QSpinBox* spinBoxWindowWidth[] = {
	//	ui->spinBoxWindowWidth0,
	//	ui->spinBoxWindowWidth1,
	//	ui->spinBoxWindowWidth2
	//};
	//m_spinBoxWindowLevel = spinBoxWindowLevel[numOfMyself - 1];
	//m_spinBoxWindowWidth = spinBoxWindowWidth[numOfMyself - 1];
	connect(m_pushButtonReset, SIGNAL(clicked()),
		this, SLOT(ResetWindowLevel()), Qt::UniqueConnection);
	connect(m_spinBoxWindowLevel, SIGNAL(valueChanged(int)),
		this, SLOT(SetLevel(int)), Qt::UniqueConnection);
	connect(m_spinBoxWindowWidth, SIGNAL(valueChanged(int)),
		this, SLOT(SetWindow(int)), Qt::UniqueConnection);
	connect(m_spinBoxWindowLevel, SIGNAL(valueChanged(int)),
		m_sliderWindowLevel, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(m_spinBoxWindowWidth, SIGNAL(valueChanged(int)),
		m_sliderWindowWidth, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(m_sliderWindowLevel, SIGNAL(valueChanged(int)),
		m_spinBoxWindowLevel, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(m_sliderWindowWidth, SIGNAL(valueChanged(int)),
		m_spinBoxWindowWidth, SLOT(setValue(int)), Qt::UniqueConnection);
}

void QInteractorStyleWindowLevel::SetCustomEnabled(bool flag)
{
	InteractorStyleWindowLevel::SetCustomEnabled(flag);
	uniqueInvoke(flag);
	double* range = GetImageViewer()->GetInput()->GetScalarRange();
	m_label->setText(GetImageViewer()->GetWindowName());
	m_spinBoxWindowLevel->setRange(range[0], range[1]);
	m_sliderWindowLevel->setRange(range[0], range[1]);
	m_spinBoxWindowWidth->setRange(range[0], range[1]);
	m_sliderWindowWidth->setRange(range[0], range[1]);
}

void QInteractorStyleWindowLevel::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleWindowLevel::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStyleWindowLevel::SetWindow(int window)
{
	if (int(GetWindow() + 0.5) != window) {
		InteractorStyleWindowLevel::SetWindow(window);
	}
}

void QInteractorStyleWindowLevel::SetLevel(int level)
{
	if (int(GetLevel() + 0.5) != level) {
		InteractorStyleWindowLevel::SetLevel(level);
	}
}

void QInteractorStyleWindowLevel::ResetWindowLevel()
{
	InteractorStyleWindowLevel::ResetWindowLevel();
}