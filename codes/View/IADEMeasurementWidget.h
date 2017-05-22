#ifndef __IADE_MEASUREMENT_WIDGET_H__
#define __IADE_MEASUREMENT_WIDGET_H__


#include <qwidget.h>
#include <qsharedpointer.h>

#include <itkGDCMImageIO.h>


namespace Ui { class IADEMeasurementWidget; }

class IADEMeasurementWidget: public QWidget
{
	Q_OBJECT;
public:
	IADEMeasurementWidget(QWidget* parent = Q_NULLPTR);
	~IADEMeasurementWidget();

	Ui::IADEMeasurementWidget* getUi() { return ui.data(); }
	itk::GDCMImageIO::Pointer info = nullptr;

	public slots:

	void slotUpdateLengthBA();
	void slotUpdateLengthVA();
	void slotUpdateDeviationBA();
	void slotUpdateDeviationVA();
	void slotUpdateMaxDiameterBA();
	void slotUpdateMaxDiameterVA();
	void slotUpdateUboguElongation();
	void slotUpdateUboguDeviation();
	void slotUpdateUboguExpansion();
	void slotUpdateUbogu();

	void slotUpdateInformation();
	void GenerateReport(QString path);
	void GenerateCSV(QString path);

private:

	QSharedPointer<Ui::IADEMeasurementWidget> ui;
};


#endif // !__IADE_MEASUREMENT_WIDGET_H__
