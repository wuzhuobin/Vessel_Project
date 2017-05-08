#ifndef __IADE_MEASUREMENT_WIDGET_H__
#define __IADE_MEASUREMENT_WIDGET_H__


#include <qwidget.h>

namespace Ui { class IADEMeasurementWidget; }

class IADEMeasurementWidget: public QWidget
{
	Q_OBJECT;
public:
	IADEMeasurementWidget(QWidget* parent =Q_NULLPTR);
	~IADEMeasurementWidget();

	Ui::IADEMeasurementWidget* getUi() { return ui; }

private:

	Ui::IADEMeasurementWidget* ui = nullptr;
};


#endif // !__IADE_MEASUREMENT_WIDGET_H__
