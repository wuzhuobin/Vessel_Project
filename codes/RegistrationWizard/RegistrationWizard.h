#ifndef REGISTRATIONWIZARD_H
#define REGISTRATIONWIZARD_H

#include <QWizard>
#include <QThread>

#include <qsharedpointer.h>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QProgressBar;
class QTreeWidget;
class QTableWidget;
class QThread;
class QTreeWidgetItem;
class QTabWidget;
class QSpinBox;
class QGroupBox;
class QRadioButton;

class FindImageThread : public QThread
{
	Q_OBJECT

public:
	explicit FindImageThread(QObject *parent = 0);
	void run();
	void setDirectory(QString dir);

public slots:
	void forceStop();

signals:
	void updateTree(QTreeWidgetItem*, QStringList* );
	void updateProgressBar(int);

private:
	QString m_dir;
	bool m_stop;

};

class DirectoryPage : public QWizardPage
{
    Q_OBJECT

public:
    DirectoryPage(QString dir = QString(), QWidget *parent = nullptr);
	DirectoryPage(QWidget *parent);

    int nextId() const;
	void setDirectory(QString dir);

public slots:
	void browse();

private:
	QLabel		*m_imageDirectoryLabel;
	QLineEdit	*m_imageDirectoryLineEdit;
	QPushButton *browseBtn;

};

class ImagePage : public QWizardPage
{
    Q_OBJECT

public:
	ImagePage(int numOfImages = 2, QWidget *parent = 0);
	ImagePage(QWidget *parent);

	void setImageModalityNames(QStringList imageModalityNames);

	~ImagePage();

    int nextId() const;

	virtual void initializePage();
	virtual bool validatePage();

public slots:
	void onUpdateTree(QTreeWidgetItem*,QStringList*);
	void onUpdateProgressBar(int);
	
	void setImages();

	void removeImages();



	void setImage1();
	void setImage2();
	void setImage3();
    void setImage4();
	void setImage5();
	void removeImage1();
	void removeImage2();
	void removeImage3();
    void removeImage4();
	void removeImage5();

private:
	QProgressBar *progressBar;
	QTreeWidget	 *treeWidget;

	QList<QLabel*> m_imageLabels;
	QList<QLineEdit*> m_imageLineEdits;
	QList<QPushButton*> m_imageSetBtns;
	QList<QPushButton*> m_imageRemoveBtns;
	QList<QSpinBox*> m_imageSpinBoxs;


	QLabel		 *image1Label;
	QLineEdit	 *image1LineEdit;
	QPushButton  *image1SetBtn;
	QPushButton	 *image1RemoveBtn;
	QSpinBox	 *image1SpinBox;

	QLabel		 *image2Label;
	QLineEdit	 *image2LineEdit;
	QPushButton  *image2SetBtn;
	QPushButton	 *image2RemoveBtn;
	QSpinBox	 *image2SpinBox;

	QLabel		 *image3Label;
	QLineEdit	 *image3LineEdit;
	QPushButton  *image3SetBtn;
	QPushButton	 *image3RemoveBtn;
	QSpinBox	 *image3SpinBox;
    
    QLabel		 *image4Label;
    QLineEdit	 *image4LineEdit;
    QPushButton  *image4SetBtn;
    QPushButton	 *image4RemoveBtn;
    QSpinBox	 *image4SpinBox;
	
	QLabel		 *image5Label;
    QLineEdit	 *image5LineEdit;
    QPushButton  *image5SetBtn;
    QPushButton	 *image5RemoveBtn;
    QSpinBox	 *image5SpinBox;

	QString lastDirectory;
	FindImageThread* thread;
};


class ConclusionPage : public QWizardPage
{
    Q_OBJECT

public:
    ConclusionPage(QWidget *parent = 0);

    void initializePage();
    int nextId() const;
    void setVisible(bool visible);

	void setTable1();
	void setTable2();
	void setTable3();
    void setTable4();
	void setTable5();
private:
    QLabel       *groupBoxLabel;
    QLabel		 *bottomLabel;
    QGroupBox    *groupBox;
    QRadioButton *sagittalButton;
    QRadioButton *coronalButton;
    QRadioButton *axialButton;
	QTabWidget	 *tabWidget;
	QTableWidget *table1Widget;
	QTableWidget *table2Widget;
	QTableWidget *table3Widget;
    QTableWidget *table4Widget;
	QTableWidget *table5Widget;
};





class RegistrationWizard : public QWizard
{
	Q_OBJECT

public:

	enum { 
		/// the first page
		PAGE_DIRECTORY = 0, 
		/// the second page
		PAGE_IMAGE = 1,
		/// the last
		PAGE_CONCLUSION = 2
	};
	/**
	 * Constructor.
	 * Constructor of RegistrationWizard.
	 * @param	dir 
	 * @param	numOfImages set #m_numOfImages, the number of lineEdits need to be created, these lineEdits
	 *			are for displaying what images are going to load
	 * @param	parent parent widget.
	 */
    RegistrationWizard(
		QString dir = QString(), 
		int numOfImages = 2,
		QWidget *parent = nullptr);
	RegistrationWizard(
		int numOfImages,
		QWidget *parent = nullptr);
	RegistrationWizard(
		QWidget *parent);
	/**
	 * Destructor. 
	 */
	~RegistrationWizard();

	void setImageModalityNames(unsigned int i, QString imageModalityName = QString());
    
	QStringList* getFileNames(unsigned int i);
	const QString getDirectory();
	/**
	 * Get the number of images needs to be loaded.
	 * @return	number of images, #m_numberOfImages. 
	 */
	int getNumberOfImages();


	QList<QStringList*> *fileNameList;
	QStringList* getFileNamesN(int n);
	QStringList* getFileNames1();
	QStringList* getFileNames2();
	QStringList* getFileNames3();
    QStringList* getFileNames4();
	QStringList* getFileNames5();
	int getTotalFileNo();
    
private slots:
	void showHelp();

private:


	int m_numOfImages;
	QStringList m_imageModalityNames;

	DirectoryPage	*directoryPage;
	ImagePage		*imagePage;
	ConclusionPage	*conclusionPage;

	QList<QSharedPointer<QStringList>> m_fileNamesList;

};

#endif
