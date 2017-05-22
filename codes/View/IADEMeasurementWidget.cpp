#include "IADEMeasurementWidget.h"
#include "ui_IADEMeasurementWidget.h"


IADEMeasurementWidget::IADEMeasurementWidget(QWidget * parent)
	:QWidget(parent), 
	ui(QSharedPointer<Ui::IADEMeasurementWidget>(new Ui::IADEMeasurementWidget))
{
	ui->setupUi(this);
}

IADEMeasurementWidget::~IADEMeasurementWidget()
{
}

void IADEMeasurementWidget::slotUpdateInformation()
{
	if (info.IsNull()) {
		return;
	}
	std::string patientID;
	std::string patientDOB;
	std::string patientGender;
	std::string scanDate;
	std::string patientName;
	info->GetValueFromTag("0010|0010", patientName);
	info->GetValueFromTag("0010|0020", patientID);
	info->GetValueFromTag("0010|0030", patientDOB);
	info->GetValueFromTag("0010|0040", patientGender);
	info->GetValueFromTag("0008|0020", scanDate);
	if (!patientName.empty()) {
		ui->tableWidgetDicom->setItem(0, 0, new QTableWidgetItem(QString::fromStdString(patientName)));
	}
	if (!patientID.empty()) {
		ui->tableWidgetDicom->setItem(1, 0, new QTableWidgetItem(QString::fromStdString(patientID)));
	}
	if (!patientDOB.empty()) {
		ui->tableWidgetDicom->setItem(2, 0, new QTableWidgetItem(QString::fromStdString(patientDOB)));
	}
	if (!patientGender.empty()) {
		ui->tableWidgetDicom->setItem(3, 0, new QTableWidgetItem(QString::fromStdString(patientGender)));
	}
	if (!scanDate.empty()) {
		ui->tableWidgetDicom->setItem(4, 0, new QTableWidgetItem(QString::fromStdString(scanDate)));
	}

}
