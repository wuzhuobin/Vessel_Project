#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>

#include <itkImage.h>

#include "ImageRegistration.h"


class MyImageManager;


class IOManager:public QObject
{
	Q_OBJECT
public:
	typedef itk::Image<double, 3> ImageType;
	IOManager(QObject* parent = nullptr);
	~IOManager();

	void enableRegistration(bool flag);
	//void setMyImageManager(MyImageManager* myImageManager);
	//void setUniqueKeys(QStringList keys);

	/**
	 * @brief	add a file name to #listOfFileNames
	 * @param	fileNames	a image fileName.
	 */
	//void addToListOfFileNames(QStringList fileNames);
	//void addToListOfFileNames(QStringList* fileNames);
	//void cleanListsOfFileNames();
	const QList<QStringList> getListOfFileNames();

	//void setFilePath(QString filePath);
	//const QString getFilePath();

public slots:

	/**
	 * @brief	add a file name to #listOfFileNames
	 * @param	fileNames	a image fileName.
	 */
	void addToListOfFileNames(QStringList fileNames);
	void addToListOfFileNames(QStringList* fileNames);
	void cleanListsOfFileNames();

	void slotOpenMultiImages();

	void slotOpenOneImage(QStringList fileNames);

	//void slotOpenSegmentationWithDiaglog();

	//void slotOpenSegmentation(QString fileName);

	//void slotSaveSegmentaitonWithDiaglog();

	//void slotSaveSegmentation(QString path);

	//void slotSaveContourWithDiaglog();
	//
	//void slotSaveContour(QString fileName);

signals:
	void finishOpenMultiImages();
	void finishOpenOneImage();
	//void finishOpenSegmentation();

protected:
	bool loadImageData(QStringList fileNames);


	ImageType::Pointer imageAlignment(ImageType::Pointer alignedTo,
		ImageType::Pointer toBeAligned);

private:
	QList<QStringList> listOfFileNames;
	//QString filePath;
	MyImageManager* myImageManager;
	bool registrationFlag = false;
	ImageRegistration registration;

};

#endif // !IO_MANAGER_H
