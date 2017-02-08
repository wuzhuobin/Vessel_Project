#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <qdebug.h>
#include <qsettings.h>
#include <qfiledialog.h>

#include <vtkRenderWindow.h>

#include "RegistrationWizard.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui = new Ui::MainWindow;
	ui->setupUi(this);

	settings = new QSettings("Setting.ini", QSettings::IniFormat, this);

	for (int i = 0; i < NUM_OF_2D_VIEWERS; ++i) {
		selectImgMenus << new QMenu(this);
	}
	ui->ULSelectImgBtn->setMenu(selectImgMenus[0]);
	ui->URSelectImgBtn->setMenu(selectImgMenus[1]);
	ui->LLSelectImgBtn->setMenu(selectImgMenus[2]);

	connect(ui->ULBtn, SIGNAL(clicked()), ui->actionImage1, SLOT(trigger()));
	connect(ui->URBtn, SIGNAL(clicked()), ui->actionImage2, SLOT(trigger()));
	connect(ui->LLBtn, SIGNAL(clicked()), ui->actionImage3, SLOT(trigger()));
	connect(ui->LRBtn, SIGNAL(clicked()), ui->actionImage4, SLOT(trigger()));
	connect(ui->ULBtn2, SIGNAL(clicked()), ui->actionFourViews, SLOT(trigger()));
	connect(ui->URBtn2, SIGNAL(clicked()), ui->actionFourViews, SLOT(trigger()));
	connect(ui->LLBtn2, SIGNAL(clicked()), ui->actionFourViews, SLOT(trigger()));
	connect(ui->LRBtn2, SIGNAL(clicked()), ui->actionFourViews, SLOT(trigger()));
	connect(ui->actionImage1, SIGNAL(triggered()), this, SLOT(slotImage()));
	connect(ui->actionImage2, SIGNAL(triggered()), this, SLOT(slotImage()));
	connect(ui->actionImage3, SIGNAL(triggered()), this, SLOT(slotImage()));
	connect(ui->actionImage4, SIGNAL(triggered()), this, SLOT(slotImage()));
	connect(ui->actionFourViews, SIGNAL(triggered()), this, SLOT(slotImage()));

	QActionGroup* actionGroup = new QActionGroup(this);
	actionGroup->addAction(ui->actionNavigation);
	actionGroup->addAction(ui->actionWindow_level);
	actionGroup->addAction(ui->acitonVOI_selection);
	actionGroup->addAction(ui->actionPaint_brush);
	actionGroup->addAction(ui->actionSeeds_placer);
	actionGroup->setExclusive(true);


	// Connection
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui->actionImport_images, SIGNAL(triggered()), this, SLOT(slotOpenNewImage()));
	connect(ui->actionImport_segmentation, SIGNAL(triggered()),
		this, SLOT(slotOpenOverlay()));
	createRecentImageActions();

}

MainWindow::~MainWindow()
{
	delete ui;
}

//void MainWindow::setRenderWindow(unsigned short i, vtkRenderWindow * renderWindow)
//{
//	switch (i)
//	{
//	case 0:
//		ui->image1View->SetRenderWindow(renderWindow);
//		break;
//	case 1:
//		ui->image2View->SetRenderWindow(renderWindow);
//		break;
//	case 2:
//		ui->image3View->SetRenderWindow(renderWindow);
//		break;
//	case 3:
//		ui->image4View->SetRenderWindow(renderWindow);
//		break;
//
//	default:
//		break;
//	}
//}

//vtkRenderWindow * MainWindow::getRenderWindow(unsigned short i)
//{
//	switch (i)
//	{
//	case 0:
//		return ui->image1View->GetRenderWindow();
//		break;
//	case 1:
//		return ui->image2View->GetRenderWindow();
//		break;
//	case 2:
//		return ui->image3View->GetRenderWindow();
//		break;
//	case 3:
//		return ui->image4View->GetRenderWindow();
//		break;
//
//	default:
//		return nullptr;
//		break;
//	}
//}

void MainWindow::slotOpenRecentImage()
{
	QCoreApplication::processEvents();

	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		imageImport(action->data().toString());
	}
}

void MainWindow::slotOpenNewImage()
{
	imageImport("");
}

void MainWindow::slotOpenOverlay()
{
		QString path = QFileDialog::getOpenFileName((this), 
			QString(tr("Save Segmentation")), ".", tr("NIFTI Images (*.nii)"));
		if (path.isEmpty())	return;
		emit signalOverlayImportLoad(path);
}

void MainWindow::slotImage()
{
	ui->image1frame->setHidden(true);
	ui->image2frame->setHidden(true);
	ui->image3frame->setHidden(true);
	ui->image4frame->setHidden(true);
	QAction *action = qobject_cast<QAction *>(sender());
	if (action == ui->actionFourViews) {
		ui->ULBtn->setHidden(false);
		ui->URBtn->setHidden(false);
		ui->LLBtn->setHidden(false);
		ui->LRBtn->setHidden(false);
		ui->ULBtn2->setHidden(true);
		ui->URBtn2->setHidden(true);
		ui->LLBtn2->setHidden(true);
		ui->LRBtn2->setHidden(true);
		ui->image1frame->setHidden(false);
		ui->image2frame->setHidden(false);
		ui->image3frame->setHidden(false);
		ui->image4frame->setHidden(false);
	}
	else if (action == ui->actionImage1) {
		ui->ULBtn->setHidden(true);
		ui->ULBtn2->setHidden(false);
		ui->image1frame->setHidden(false);
		ui->image2frame->setHidden(true);
		ui->image3frame->setHidden(true);
		ui->image4frame->setHidden(true);
	}
	else if (action == ui->actionImage2) {
		ui->URBtn->setHidden(true);
		ui->URBtn2->setHidden(false);
		ui->image1frame->setHidden(true);
		ui->image2frame->setHidden(false);
		ui->image3frame->setHidden(true);
		ui->image4frame->setHidden(true);
	}
	else if (action == ui->actionImage3) {
		ui->LLBtn->setHidden(true);
		ui->LLBtn2->setHidden(false);
		ui->image1frame->setHidden(true);
		ui->image2frame->setHidden(true);
		ui->image3frame->setHidden(false);
		ui->image4frame->setHidden(true);
	}
	else if (action == ui->actionImage4)
	{
		ui->LRBtn->setHidden(true);
		ui->LRBtn2->setHidden(false);
		ui->image1frame->setHidden(true);
		ui->image2frame->setHidden(true);
		ui->image3frame->setHidden(true);
		ui->image4frame->setHidden(false);
	}
	//m_core->RenderAllViewer();
}

void MainWindow::imageImport(QString path)
{
	RegistrationWizard rw(path, 2, this);
	rw.setImageModalityNames(0, modalityNames[0]);
	rw.setImageModalityNames(1, modalityNames[1]);

	QList<QStringList> _listOfFileNames;

	if (QWizard::Accepted == rw.exec()) {

		if (rw.getFileNames(0)) {
			qDebug() << *rw.getFileNames(0);
			_listOfFileNames << *rw.getFileNames(0);
		}
		if (rw.getFileNames(1)) {
			qDebug() << *rw.getFileNames(1);
			_listOfFileNames << *rw.getFileNames(1);
		}

		emit signalImageImportLoad(&_listOfFileNames);

		qDebug() << rw.getDirectory();

		adjustForCurrentFile(rw.getDirectory());

	}
}

void MainWindow::initialization()
{
	this->setEnabled(true);

	ui->centralwidget->setEnabled(true);

	ui->ActionToolBar->setEnabled(true);

	ui->image1frame->setEnabled(true);
	ui->image2frame->setEnabled(true);
	ui->image3frame->setEnabled(true);
	ui->image4frame->setEnabled(true);
	ui->image1View->setEnabled(true);
	ui->image1View->setEnabled(true);
	ui->image2View->setEnabled(true);
	ui->image3View->setEnabled(true);
	ui->image4View->setEnabled(true);


	ui->actionNavigation->trigger();

}

//void MainWindow::setModuleWidget(QWidget * moduleWidget)
//{
//	ui->moduleWidgetDockWidget->setWidget(moduleWidget);
//}

void MainWindow::addModalityNames(QString name)
{
	modalityNames << name;
	for (int i = 0; i < NUM_OF_2D_VIEWERS; ++i) {
			selectImgMenus[i]->addAction(new QAction(name, selectImgMenus[i]));
	}
}

void MainWindow::setSelectImgMenuVisible(unsigned int num, bool flag)
{
	for (int i = 0; i < NUM_OF_2D_VIEWERS; ++i) {
		QList<QAction*> actions = selectImgMenus[i]->actions();
		actions[num]->setVisible(flag);
	}
}

void MainWindow::clearModalityNames()
{
	modalityNames.clear();
	for (int i = 0; i < NUM_OF_2D_VIEWERS; ++i) {
		selectImgMenus[i]->clear();
	}
}


void MainWindow::setEnabled(bool flag)
{
	ui->actionAbout->setEnabled(true);

}

void MainWindow::createRecentImageActions()
{
	QAction* recentFileAction = 0;
	for (int i = 0; i < MAX_RECENT_IMAGE; i++) {
		recentFileAction = new QAction(this);
		recentFileAction->setVisible(false);
		connect(recentFileAction, SIGNAL(triggered()), this, SLOT(slotOpenRecentImage()));

		recentFileActionList.append(recentFileAction);
		ui->menuRecentImage->addAction(recentFileAction);
	}

	updateRecentActionList();
}

void MainWindow::adjustForCurrentFile(const QString &filePath)
{
	QStringList recentFilePaths = settings->value("recentFiles").toStringList();

	recentFilePaths.removeAll(filePath);
	recentFilePaths.prepend(filePath);
	while (recentFilePaths.size() > MAX_RECENT_IMAGE)
		recentFilePaths.removeLast();
	settings->setValue("recentFiles", recentFilePaths);

	// see note
	updateRecentActionList();
}

void MainWindow::updateRecentActionList()
{
	QStringList recentFilePaths =
		settings->value("recentFiles").toStringList();

	int itEnd = 0;
	if (recentFilePaths.size() <= MAX_RECENT_IMAGE)
		itEnd = recentFilePaths.size();
	else
		itEnd = MAX_RECENT_IMAGE;

	for (int i = 0; i < itEnd; i++)
	{
		recentFileActionList.at(i)->setText(recentFilePaths.at(i));
		recentFileActionList.at(i)->setData(recentFilePaths.at(i));
		recentFileActionList.at(i)->setVisible(true);
	}

	for (int i = itEnd; i < MAX_RECENT_IMAGE; i++)
		recentFileActionList.at(i)->setVisible(false);
}