#pragma once
#ifndef __MEASUREMENTWIDGET_H__
#define __MEASUREMENTWIDGET_H__
#include <QWidget>

//#include "ui_measurementwidget.h"

namespace Ui { class MeasurementWidget; }

class MainWindow;

class MeasurementWidget : public QWidget {
	Q_OBJECT

public:
	MeasurementWidget(QWidget * parent = Q_NULLPTR);
	~MeasurementWidget();

	//void UpdateMeasurementsForObliqueSlice(vtkImageData*);
	Ui::MeasurementWidget* getUi();

public slots:
	void slotUpdate3DMeasurements(double* Measurements3D);
	void slotUpdate2DMeasurements(double* Measurements2D);
	void slotUpdate2DMeasurements(int slice);
	void slotUpdateStenosis(double stenosis);
	void slotUpdateImformation();
	void slotReportGetInput();

private:
	Ui::MeasurementWidget* ui = nullptr;

	void GenerateReport();

	friend class MainWindow;
	MainWindow* m_mainWnd;


};

#endif // __MEASUREMENTWIDGET_H__