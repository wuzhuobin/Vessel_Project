#include "Measurement.h"
#include "PlaqueQuantOverlay.h"

class PlaqueQuantMeasurement: public Measurement
{
	Q_OBJECT;
public:
	PlaqueQuantMeasurement(QObject* parent);
	~PlaqueQuantMeasurement();

	virtual PlaqueQuantOverlay* getPlaqueQuantOverlay();

	// total plaque, vessel wall, lumen, calcification, hemorrhage, lrnc, lm,
	double m_measurements3D[PlaqueQuantOverlay::NUMBER_OF_COLOR] = { 0 };
	// vessel wall, lumen, NMI
	//double m_measurements2D[4] = { 0 };
	QMap<int, QSharedPointer<double>> m_measurements2DMap;
	double m_stenosis = 0;
	//int m_currentSlice = 0;

public slots:
	virtual void update() override;
	virtual void updateMeasurement3D();
	virtual void updateMeasurement2D();
	/**
	 * @deprecated
	 */
	virtual void updateMeasurement2D(int slice);
	virtual void updateStenosis();
	/**
	 * @deprecated
	 */
	//virtual void setSlice(int slice);

protected:
	//unsigned long overlayMTime2D = 0;
	//unsigned long overlayMTime3D = 0;

private:

};
