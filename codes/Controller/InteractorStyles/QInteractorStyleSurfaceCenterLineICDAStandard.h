#ifndef __QINTERACTOR_STYLE_SURFACE_CENTERLINE_ICDA_STANDARD_H__
#define __QINTERACTOR_STYLE_SURFACE_CENTERLINE_ICDA_STANDARD_H__

#include "QAbstractInteractorStyle.h"
#include "InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius.h"

namespace Ui { class QInteractorStyleSurfaceCenterLineICDAStandard; }

class QInteractorStyleSurfaceCenterLineICDAStandard: 
	public QAbstractInteractorStyle, public InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius 
{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleSurfaceCenterLineICDAStandard);

public:

	enum MEASURE_MODE
	{
		CAVERNOUS_SEGMENT_OF_INTERNAL_CAROTID_ARTERY = 0,
		MIDDLE_CEREBRAL_ARTERY = 1,
		ANTERIOR_CEREBRAL_ARTERY = 2
	};



	static QInteractorStyleSurfaceCenterLineICDAStandard* New();
	vtkTypeMacro(QInteractorStyleSurfaceCenterLineICDAStandard, InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius);


protected:
	QInteractorStyleSurfaceCenterLineICDAStandard(int uiType = MULTIPLE_UI, QWidget* parent = Q_NULLPTR);
	virtual ~QInteractorStyleSurfaceCenterLineICDAStandard() override;



	void initialization();
	
private:
	///< ui thing. For integrity.
	Ui::QInteractorStyleSurfaceCenterLineICDAStandard* ui = nullptr;
};



#endif // !__QINTERACTOR_STYLE_SURFACE_CENTERLINE_ICDA_STANDARD_H__

