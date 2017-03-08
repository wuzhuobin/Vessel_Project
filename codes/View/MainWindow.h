/**
* MainWindow of 
* API class for using the Vikey USB authentication system. The class is derived from
* QWidget. And this API class can only be used under Windows OS.
*
* Updated Log:
* version 1.0.0
* Original one provided authentication method for having key, soft id, hard id
* product name, expired date time
*
* @class	EncryptionAuthentication
* @author	WUZHUOBIN
* @version	1.1.0
* @since	2017.01.05
*
*/

#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <QtWidgets/QMainWindow>



namespace Ui { class MainWindow; }
class vtkRenderWindow;
class QMenu;
class QSettings;


class MainWindow : public QMainWindow
{
	Q_OBJECT;


public:

	const static unsigned short NUM_OF_2D_VIEWERS = 3;
	const static unsigned short NUM_OF_3D_VIEWERS = 1;

	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void initialization();
	void enableInteractor(bool flag);
	//void setModuleWidget(QWidget* moduleWidget);

	void addModalityNames(QString name);
	void setSelectImgMenuVisible(unsigned int num, bool flag);
	void clearModalityNames();

	
	Ui::MainWindow* getUi() { return ui; }
	QMainWindow* getCentralWidget();
	QMenu* getSelectImgMenu(unsigned int i);
	QList<QMenu*>* getSelectImgMenus();

	//void setRenderWindow(unsigned short i, vtkRenderWindow* renderWindow);
	//vtkRenderWindow* getRenderWindow(unsigned short i);

signals:
	void signalImageImportInitialize();
	void signalImageImportAdd(QStringList*);
	void signalImageImportLoad();
	void signalImageImportLoad(QList<QStringList>*);
	void signalOverlayImportLoad(QString);
	void signalOverlayExportSave(QString);


private slots:
	
	void slotOpenRecentImage();
	void slotOpenNewImage();

	void slotOpenOverlay();
	void slotSaveOverlay();

	/**
	* four viewers and maximum
	*/
	void slotImage();


private:
	void imageImport(QString path);


	void setEnabled(bool flag);

	//Recent File
	const static int MAX_RECENT_IMAGE = 10;
	QList<QAction*> recentFileActionList;
	void createRecentImageActions();
	void adjustForCurrentFile(const QString& filePath);
	void updateRecentActionList();



	Ui::MainWindow* ui;

	QMainWindow* centralWidget = nullptr;

	QList<QMenu*> selectImgMenus;

	QStringList modalityNames;


	QSettings* settings;
	//RegistrationWizard* m_rw;
};

#endif // !__MAIN_WINDOW_H__
