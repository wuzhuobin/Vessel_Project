#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui = new Ui::MainWindow;
	ui->setupUi(this);


	// Connection
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
	delete ui;
}
