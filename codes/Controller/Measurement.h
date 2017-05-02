#include <qobject.h>
#include <qsharedpointer.h>

#include "Overlay.h"

class Measurement : public QObject
{
	Q_OBJECT;
public:
	Measurement(QObject* parent = nullptr);
	virtual ~Measurement();

	virtual void setOverlay(Overlay* overlay);
	virtual Overlay* getOverlay();
public slots:
	virtual void update();
signals:
	void updated();

protected:
	Overlay* m_overlay = nullptr;
	


};