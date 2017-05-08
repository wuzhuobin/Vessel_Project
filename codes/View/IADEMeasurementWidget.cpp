#include "IADEMeasurementWidget.h"
#include "ui_IADEMeasurementWidget.h"

IADEMeasurementWidget::IADEMeasurementWidget(QWidget * parent)
	:QWidget(parent)
{
	ui = new Ui::IADEMeasurementWidget;
	ui->setupUi(this);
}

IADEMeasurementWidget::~IADEMeasurementWidget()
{
}
