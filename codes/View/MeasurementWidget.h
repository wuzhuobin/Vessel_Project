#pragma once
#ifndef __MEASUREMENTWIDGET_H__
#define __MEASUREMENTWIDGET_H__
#include <QWidget>

//#include "ui_measurementwidget.h"
#include <itkGDCMImageIO.h>

namespace Ui { class MeasurementWidget; }

class MainWindow;
class vtkRenderWindow;

class MeasurementWidget : public QWidget {
	Q_OBJECT

public:
	MeasurementWidget(QWidget * parent = Q_NULLPTR);
	~MeasurementWidget();

	Ui::MeasurementWidget* getUi();
	itk::GDCMImageIO::Pointer info = nullptr;
	vtkRenderWindow* wind1 = nullptr;
	vtkRenderWindow* wind2 = nullptr;
	double* measurements3D = nullptr;
	//double* measurements2D = nullptr;
	QMap<int, QSharedPointer<double>>* measurements2DMap = nullptr;

	double* stenosis = nullptr;


public slots:
	void slotUpdateMeasurements(int slice);
	void slotUpdate3DMeasurements(double* Measurements3D);
	void slotUpdate2DMeasurements(double* Measurements2D);
	void slotUpdateStenosis(double* stenosis);
	void slotUpdateImformation();

private:
	Ui::MeasurementWidget* ui = nullptr;
	void GenerateReport(QString	path);

};

#endif // __MEASUREMENTWIDGET_H__