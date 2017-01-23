#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <qdebug.h>
#include <qsettings.h>


#include "RegistrationWizard.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui = new Ui::MainWindow;
	ui->setupUi(this);



	// Connection
	connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui->actionImport_images, SIGNAL(triggered()), this, SLOT(slotOpenNewImage()));

	createRecentImageActions();
}

MainWindow::~MainWindow()
{
	delete ui;
}

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

void MainWindow::imageImport(QString path)
{
	emit signalImageImportInitialize();
	RegistrationWizard rw(path, 2, this);
	rw.setImageModalityNames(0, "T2 image ");
	rw.setImageModalityNames(1, "MRA image");
	if (QWizard::Accepted == rw.exec()) {

		if (rw.getFileNames(0)) {
			qDebug() << *rw.getFileNames(0);
			emit signalImageImportAdd(rw.getFileNames(0).data());
		}
		if (rw.getFileNames(1)) {
			qDebug() << *rw.getFileNames(1);
			emit signalImageImportAdd(rw.getFileNames(1).data());
		}
		emit signalImageImportLoad();
		qDebug() << rw.getDirectory();
		adjustForCurrentFile(rw.getDirectory());

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
	QSettings settings("DIIR", "IADE_Analyzer");
	QStringList recentFilePaths = settings.value("recentFiles").toStringList();

	recentFilePaths.removeAll(filePath);
	recentFilePaths.prepend(filePath);
	while (recentFilePaths.size() > MAX_RECENT_IMAGE)
		recentFilePaths.removeLast();
	settings.setValue("recentFiles", recentFilePaths);

	// see note
	updateRecentActionList();
}

void MainWindow::updateRecentActionList()
{
	QSettings settings("DIIR", "IADE_Analyzer");
	QStringList recentFilePaths =
		settings.value("recentFiles").toStringList();

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