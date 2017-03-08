#include "ViewerWidget.h"

#include "ui_ViewerWidget.h"

ViewerWidget::ViewerWidget(QWidget * parent)
	:QDockWidget(parent, Qt::Window | Qt::WindowMinMaxButtonsHint)
{

	//setWindowFlags(Qt::CustomizeWindowHint);
	//setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
	QWidget* w = new QWidget(this);
	//w->setWindowFlags( Qt::WindowMinMaxButtonsHint);
	ui = new Ui::ViewerWidget;
	ui->setupUi(w);
	setWidget(w);

	setFeatures(DockWidgetFloatable | DockWidgetMovable);
	setAllowedAreas(Qt::AllDockWidgetAreas);
}

Ui::ViewerWidget * ViewerWidget::getUi()
{
	return this->ui;
}
