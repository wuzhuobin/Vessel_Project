#include "QInteractorStyleWindowLevelThreshold.h"
#include "ui_QInteractorStyleWindowLevelThreshold.h"
#include "ui_QAbstractNavigation.h"
#include "ImageViewer.h"

#include <vtkImageData.h>
#include <vtkObjectFactory.h>

#include <qpushbutton.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qlabel.h>

vtkStandardNewMacro(QInteractorStyleWindowLevelThreshold);
QSETUP_UI_SRC(QInteractorStyleWindowLevelThreshold);
using namespace std;
QInteractorStyleWindowLevelThreshold::QInteractorStyleWindowLevelThreshold(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleWindowLevelThreshold::~QInteractorStyleWindowLevelThreshold()
{
	QDELETE_UI();
}

void QInteractorStyleWindowLevelThreshold::uniqueEnable()
{
	QAbstractNavigation::uniqueEnable();
	UpdateTargetViewer();
}

void QInteractorStyleWindowLevelThreshold::SetWindowByViewer(double window)
{
	InteractorStyleWindowLevelThreshold::SetWindowByViewer(window);
	//if (int(m_window + 0.5) != window) {
		m_spinBoxWindowWidth->setValue(window);
	//}
}

void QInteractorStyleWindowLevelThreshold::SetLevelByViewer(double level)
{
	InteractorStyleWindowLevelThreshold::SetLevelByViewer(level);
	//if (int(m_level + 0.5) != level) {
		m_spinBoxWindowLevel->setValue(level);
	//}
}

void QInteractorStyleWindowLevelThreshold::UpdateTargetViewer()
{
	//QStringList listOfModalityName;
	ui->comboBoxTargeImage->clear();
	m_listOfModalityNames.clear();
	for (list<AbstractInteractorStyleImage*>::const_iterator cit = m_imageStyles.cbegin();
		cit != m_imageStyles.cend(); ++cit) {
		QInteractorStyleWindowLevelThreshold* _style = QInteractorStyleWindowLevelThreshold::SafeDownCast(*cit);
		// because not all have been CustomEnabled this time
		if (_style /*&& _style->GetCustomEnabled()*/) {
			m_listOfModalityNames.append(QString::fromStdString(_style->GetImageViewer()->GetWindowName()));
		}
	}
	m_listOfModalityNames.removeDuplicates();
	//if (m_listOfModalityNames != listOfModalityName)
	//{
	for (int i = 0; i < m_listOfModalityNames.size(); ++i) {
		ui->comboBoxTargeImage->addItem(m_listOfModalityNames[i]);
	}
	//}
}

void QInteractorStyleWindowLevelThreshold::initialization()
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

	// set brushShape
	connect(ui->labelComboBox, SIGNAL(currentIndexChanged(int)),
		this, SLOT(SetOutputLabel(int)));
}

void QInteractorStyleWindowLevelThreshold::uniqueInitialization()
{
	connect(ui->pushButtonThreshold, SIGNAL(clicked()),
		this, SLOT(ThresholdTargetViewerToOverlay()));
}

void QInteractorStyleWindowLevelThreshold::SetCustomEnabled(bool flag)
{
	InteractorStyleWindowLevelThreshold::SetCustomEnabled(flag);
	uniqueInvoke(flag);
	if (flag) {
		double* range = GetImageViewer()->GetInput()->GetScalarRange();
		m_label->setText(GetImageViewer()->GetWindowName());
		m_spinBoxWindowLevel->setRange(range[0], range[1]);
		m_sliderWindowLevel->setRange(range[0], range[1]);
		m_spinBoxWindowWidth->setRange(range[0], range[1]);
		m_sliderWindowWidth->setRange(range[0], range[1]);
		m_spinBoxWindowWidth->setValue(GetWindow());
		m_spinBoxWindowLevel->setValue(GetLevel());
	}
}

void QInteractorStyleWindowLevelThreshold::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleWindowLevelThreshold::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStyleWindowLevelThreshold::SetWindow(int window)
{
	if (int(GetWindow() + 0.5) != window) {
		InteractorStyleWindowLevelThreshold::SetWindow(window);
	}
}

void QInteractorStyleWindowLevelThreshold::SetLevel(int level)
{
	if (int(GetLevel() + 0.5) != level) {
		InteractorStyleWindowLevelThreshold::SetLevel(level);
	}
}

void QInteractorStyleWindowLevelThreshold::ThresholdTargetViewerToOverlay()
{

	for (list<AbstractInteractorStyleImage*>::const_iterator cit = m_imageStyles.cbegin();
		cit != m_imageStyles.cend(); ++cit) {
		QInteractorStyleWindowLevelThreshold* _style = QInteractorStyleWindowLevelThreshold::SafeDownCast(*cit);
		if (_style && _style->GetCustomEnabled() && 
			QString::fromStdString(_style->GetImageViewer()->GetWindowName()) ==
			ui->comboBoxTargeImage->currentText()) {
			_style->ThresholdToOverlay();
			break;
		}
	}
}

void QInteractorStyleWindowLevelThreshold::SetOutputLabel(int label)
{
	InteractorStyleWindowLevelThreshold::SetOutputLabel(label + 1);
}

void QInteractorStyleWindowLevelThreshold::ResetWindowLevel()
{
	InteractorStyleWindowLevelThreshold::ResetWindowLevel();
}