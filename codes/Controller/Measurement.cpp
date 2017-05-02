#include "Measurement.h"

Measurement::Measurement(QObject * parent)
	: QObject(parent)
{
}

Measurement::~Measurement()
{
}

void Measurement::setOverlay(Overlay* overlay)
{
	if(m_overlay != overlay){
		//disconnect(m_overlay, SIGNAL(signalUpdatedOverlay()),
		//	this, SLOT(update()));
		m_overlay = overlay;
		connect(m_overlay, SIGNAL(signalUpdatedOverlay()),
			this, SLOT(update()));
	}
}

Overlay* Measurement::getOverlay()
{
	return m_overlay;
}

void Measurement::update()
{
	emit updated();
}
