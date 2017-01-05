/**
* API class for using the Vikey USB authentication
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

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow* ui;
};

#endif // !__MAIN_WINDOW_H__
