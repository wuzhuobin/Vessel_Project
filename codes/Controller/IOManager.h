#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>
#include <qsharedpointer.h>

#include <itkImage.h>
#include <itkGDCMImageIO.h>

#include "ImageRegistration.h"
#include "Overlay.h"
#include "IVtkImageData.h"


class IOManager:public QObject
{
	Q_OBJECT;
public:
	//typedef itk::Image<float, 3> ImageType;
	//typedef itk::Image<unsigned char, 3> OverlayType;

	IOManager(QObject* parent = nullptr);
	~IOManager();

	void enableRegistration(bool flag);

	const QList<QStringList> getListOfFileNames() const;

	/**
	* clean the list of FileNames, #listOfFileNames
	*/
	void clearListsOfFileNames();

	//const QList<ImageType::Pointer> getListOfItkImages() const;
	const QList<vtkSmartPointer<IVtkImageData>> getListOfImage() const;
	/**
	 * 
	 */
	//void clearListOfItkImages();

	const QList<itk::GDCMImageIO::Pointer> getListOfDicomIOs() const;

	void clearListOfDicoms();

	//const OverlayType::Pointer getOverlay() const;

	const QSharedPointer<Overlay> getOverlay() const;

	//void clearOverlay();

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

	virtual void slotInitializeOverlay();

	virtual void slotInitializeOverlay(IVtkImageData::itkImageType::Pointer image);

	//void slotOpenSegmentationWithDiaglog();

	virtual void slotOpenSegmentation(QString fileName);

	//void slotSaveSegmentaitonWithDiaglog();

	void slotSaveSegmentation(QString path);

	void slotSaveSegmentation(OverlayImageData::itkImageType::Pointer input, QString path);


	//void slotSaveContourWithDiaglog();
	//
	//void slotSaveContour(QString fileName);

signals:
	void signalFinishOpenMultiImages();

	//void signalFinishOpenMultiImages(QList<ImageType::Pointer>*, QList<itk::GDCMImageIO::Pointer>*);

	void signalFinishOpenOneImage();

	void signalFinishOpenOverlay();
	//void finishOpenSegmentation();

protected:
	bool loadImageData(QStringList fileNames);


	IVtkImageData::itkImageType::Pointer imageAlignment(IVtkImageData::itkImageType::Pointer alignedTo,
		IVtkImageData::itkImageType::Pointer toBeAligned);

//private:
	QList<QStringList> listOfFileNames;
	//QList<ImageType::Pointer> listOfItkImages;
	QList<vtkSmartPointer<IVtkImageData>> listOfImages;
	QList<itk::GDCMImageIO::Pointer> listOfDicomIOs;

	//ImageType::Pointer overlay;
	//OverlayType::Pointer overlay;
	QSharedPointer<Overlay> overlay;

	//QString filePath;
	bool registrationFlag = false;
	ImageRegistration registration;

};

#endif // !IO_MANAGER_H
