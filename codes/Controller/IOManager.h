#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>

#include <itkImage.h>
#include <itkGDCMImageIO.h>

#include "ImageRegistration.h"


class IOManager:public QObject
{
	Q_OBJECT;
public:
	typedef itk::Image<float, 3> ImageType;

	IOManager(QObject* parent = nullptr);
	~IOManager();

	void enableRegistration(bool flag);

	const QList<QStringList> getListOfFileNames() const;

	/**
	* clean the list of FileNames, #listOfFileNames
	*/
	void clearListsOfFileNames();

	const QList<ImageType::Pointer> getListOfItkImages() const;

	/**
	 * 
	 */
	void clearListOfItkImages();

	const QList<itk::GDCMImageIO::Pointer> getListOfDicomIOs() const;

	void clearListOfDicoms();

	const ImageType::Pointer getOverlay() const;

	void clearOverlay();

public slots:

	void slotAddToListOfFileNamesAndOpen(QList<QStringList>* listOfFileNames);
	/**
	 * @brief	add a file name to #listOfFileNames
	 * @param	fileNames	a image fileName.
	 */
	void slotAddToListOfFileNames(QStringList fileNames);

	void slotAddToListOfFileNames(QStringList* fileNames);


	void slotOpenMultiImages();

	void slotOpenOneImage(QStringList fileNames);

	void slotCleanImagesAndDicomIOs();

	void slotInitializeOverlay();

	void slotInitializeOverlay(ImageType::Pointer image);

	//void slotOpenSegmentationWithDiaglog();

	void slotOpenSegmentation(QString fileName);

	//void slotSaveSegmentaitonWithDiaglog();

	//void slotSaveSegmentation(QString path);

	//void slotSaveContourWithDiaglog();
	//
	//void slotSaveContour(QString fileName);

signals:
	void signalFinishOpenMultiImages();

	void signalFinishOpenMultiImages(QList<ImageType::Pointer>*, QList<itk::GDCMImageIO::Pointer>*);

	void signalFinishOpenOneImage();

	void signalFinishOpenOverlay();
	//void finishOpenSegmentation();

protected:
	bool loadImageData(QStringList fileNames);


	ImageType::Pointer imageAlignment(ImageType::Pointer alignedTo,
		ImageType::Pointer toBeAligned);

private:
	QList<QStringList> listOfFileNames;
	QList<ImageType::Pointer> listOfItkImages;
	QList<itk::GDCMImageIO::Pointer> listOfDicomIOs;

	ImageType::Pointer overlay;

	//QString filePath;
	bool registrationFlag = false;
	ImageRegistration registration;

};

#endif // !IO_MANAGER_H
