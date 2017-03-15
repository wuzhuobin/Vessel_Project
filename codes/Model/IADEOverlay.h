#ifndef __IADE_OVERLAY_H__
#define __IADE_OVERLAY_H__

#include "Overlay.h"
#include <qmap.h>
#include <qsharedpointer.h>


class IADEOverlay : public Overlay 
{
	Q_OBJECT;

	//struct Measurements2D
	//{
	//	double vesselWall;
	//	double lumen;
	//	double NMI;
	//};

	//struct Measurements3D
	//{
	//	double vesselWall;
	//	double lumen;
	//	double calcification;
	//	double hemorrhage;
	//	double lrnc;
	//	double lm;

	//};

public:


	static const int NUMBER_OF_COLOR = 7;
	const int* m_overlayColor[NUMBER_OF_COLOR];
	const int zeros[4] = { 0,0,0,0 };
	const int vesselWall[4] = { 0, 0, 255, 255 };
	const int lumen[4] = { 255, 0, 0, 255 };
	const int calcification[4] = { 0,255,0,255 };
	const int hemorrhage[4] = { 255, 255, 0,255 };
	const int lrnc[4] = { 0, 255, 255 ,255 };
	const int lm[4] = { 255, 0, 255 ,255 };


	IADEOverlay(QObject* parent = nullptr);
	IADEOverlay(OverlayImageData::itkImageType::Pointer data, QObject* parent = nullptr);
	IADEOverlay(OverlayImageData* data, QObject* parent = nullptr);


	// total plaque, vessel wall, lumen, calcification, hemorrhage, lrnc, lm,
	double Measurements3D[7] = { 0 };
	// vessel wall, lumen, NMI
	//double Measurements2D[3] = { 0 };
	int currentSlice = 0;;
	QMap<int, QSharedPointer<double>> Measurements2D;
	public slots:
	virtual void updateMeasurement3D();
	virtual void SetCurrentSlice(int slice);
	virtual void updateMeasurement2D(int slice);

protected:
	virtual void updatedOverlay() override;


};



#endif // !__IADE_OVERLAY_H__
