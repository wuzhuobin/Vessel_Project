#ifndef __VIEWER_WIDGET_H__
#define __VIEWER_WIDGET_H__

#include <qdockwidget.h>


namespace Ui { class ViewerWidget; }

class ViewerWidget: public QDockWidget
{
	Q_OBJECT;
public:
	ViewerWidget(QWidget* parent = nullptr);
	Ui::ViewerWidget* getUi();


private:
	Ui::ViewerWidget* ui = nullptr;
};


#endif // !__VIEWER_WIDGET_H__
