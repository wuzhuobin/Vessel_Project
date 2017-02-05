#ifndef MODULEWIDGET_H
#define MODULEWIDGET_H

#include <QWidget>



namespace Ui { class ModuleWidget; }

class ModuleWidget: public QWidget
{
    Q_OBJECT

public:
    explicit ModuleWidget(QWidget *parent = 0);
    ~ModuleWidget();

	void ClearWidget();
	void addWidget(QWidget* style);
	void setWidget(QWidget* style);

public slots:
	void slotChangeOpacity(int opactiy);
	void slotUpdateOpacity();


    
private:

    Ui::ModuleWidget *ui;
	
};

#endif // MODULEWIDGET_H
