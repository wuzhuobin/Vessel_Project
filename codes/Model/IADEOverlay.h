#ifndef __IADE_OVERLAY_H__
#define __IADE_OVERLAY_H__

#include "Overlay.h"




class IADEOverlay : public Overlay 
{
	Q_OBJECT;

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




};



#endif // !__IADE_OVERLAY_H__
