#ifndef REGISTRATIONWIZARD_H
#define REGISTRATIONWIZARD_H

#include <QWizard>
#include <QThread>
#include <qlist.h>

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
//class QSpinBox;
class QGroupBox;
class QRadioButton;
class ImagePage;

class FindImageThread : public QThread
{
	Q_OBJECT

public:
	explicit FindImageThread(QObject *parent = 0);
	void run();
	void setDirectory(QString dir);

	void setImagePage(ImagePage* imagePage);
	void updateTreeWidget(QStringList* imagePath, QString format);


signals:
	void updateProgressBar(int);

public slots:
	void forceStop();

private:

	ImagePage* m_imagePage;
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
	ImagePage(int numOfImages = 2, QWidget* parent = nullptr);
	ImagePage(QWidget *parent);

	QList<QSharedPointer<QStringList>>* m_imagePaths;
	QList<int>* m_selectedImages;

	void setImageModalityNames(unsigned int i, QString imageModalityName);

    int nextId() const;

	virtual void initializePage();
	virtual bool validatePage();

public slots:
	void onUpdateProgressBar(int);
	
	void setImages();

	void removeImages();

private:
	QProgressBar *progressBar;
	QTreeWidget	 *treeWidget;

	QList<QLabel*> m_imageLabels;
	QList<QLineEdit*> m_imageLineEdits;
	QList<QPushButton*> m_imageSetBtns;
	QList<QPushButton*> m_imageRemoveBtns;
	//QList<QSpinBox*> m_imageSpinBoxs;


	QString lastDirectory;
	friend FindImageThread;
	FindImageThread* thread;
};


class ConclusionPage : public QWizardPage
{
    Q_OBJECT

public:
    ConclusionPage(int numOfImages = 2, QWidget* parent = nullptr);
	ConclusionPage(QWidget* parent);

	//QList<QSharedPointer<QMap<QString, QString>>>* m_DICOMHeaders;
	QList<QSharedPointer<QStringList>>* m_imagePaths;
	QList<int>* m_selectedImages;



    void initializePage();
    int nextId() const;

private:
    QLabel		 *bottomLabel;
	QTabWidget	 *tabWidget;

	QList<QTableWidget*> m_tableWidgets;
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
	int getTotalFileNo();
    
private slots:
	void showHelp();

private:


	int m_numOfImages;

	DirectoryPage	*directoryPage;
	ImagePage		*imagePage;
	ConclusionPage	*conclusionPage;

	QList<QSharedPointer<QStringList>> m_fileNamesList;
	QList<QSharedPointer<QStringList>> m_imagePaths;
	QList<int> m_selectedImages;

};

#endif
