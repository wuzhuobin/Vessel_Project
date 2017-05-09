#ifndef __QINTERACTOR_STYLE_SURFACE_CENTERLINE_VBD_UBOGU_STANDARD_H__
#define __QINTERACTOR_STYLE_SURFACE_CENTERLINE_VBD_UBOGU_STANDARD_H__

#include "InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius.h"
#include "QAbstractInteractorStyle.h"

#include <vtkSmartPointer.h>

namespace Ui { class QInteractorStyleSurfaceCenterLineVBDUboguStandard; }
class PerpendicularMeasurementLineWidget;

class QInteractorStyleSurfaceCenterLineVBDUboguStandard:
	public QAbstractInteractorStyle, public InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius

{
	Q_OBJECT;
	QSETUP_UI_HEAD(QInteractorStyleSurfaceCenterLineVBDUboguStandard);
public:

	enum MEASURE_MODE
	{
		LENGTH_OF_BASILAR_ARTERY = 0,
		LENGTH_OF_INTRACRANIAL_SEGMENT_OF_LEFT_VERTEBRAL_ARTERY = 1,
		LENGTH_OF_INTRACRAINAL_SEGMENT_OF_RIGHT_VERTEBRAL_ARTERY = 2,
		DISTANCE_BETWEEN_CENTERLINE_AND_CONNECTION_LEAD_OF_2_TERMINALS_LEFT = 3,
		DISTANCE_BETWEEN_CENTERLINE_AND_CONNECTION_LEAD_OF_2_TERMINALS_RIGHT = 4,
		MINIMUM_DIAMETER_OF_BASILAR_ARTERY = 5,
		MINIMUM_DIAMETER_OF_LEFT_VERTEBRAL_ARTERY = 6,
		MINIMUM_DIAMETER_OF_RIGHT_VERTEBRAL_ARTERY = 7
	};

	static QInteractorStyleSurfaceCenterLineVBDUboguStandard* New();
	vtkTypeMacro(QInteractorStyleSurfaceCenterLineVBDUboguStandard, InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius);


	public slots:
	virtual void SetCustomEnabled(bool flag) override;
	virtual void setCurrentMode(int mode);
	virtual void setCurrentModeToLengthOfBasilarArtery() { setCurrentMode(LENGTH_OF_BASILAR_ARTERY); }
	virtual void setCurrentModeToLengthOfIntracranialSegmentOfLeftVertebralArtery() { setCurrentMode(LENGTH_OF_INTRACRANIAL_SEGMENT_OF_LEFT_VERTEBRAL_ARTERY); }
	virtual void setCurrentModeToLengthOfIntracranialSegmentOfRightVertebralArtery() { setCurrentMode(LENGTH_OF_INTRACRAINAL_SEGMENT_OF_RIGHT_VERTEBRAL_ARTERY); }
	virtual void setCurrentModeToDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsLeft() { setCurrentMode(DISTANCE_BETWEEN_CENTERLINE_AND_CONNECTION_LEAD_OF_2_TERMINALS_LEFT); }
	virtual void setCurrentModeToDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsRight() { setCurrentMode(DISTANCE_BETWEEN_CENTERLINE_AND_CONNECTION_LEAD_OF_2_TERMINALS_RIGHT); }
	virtual void setCurrentModeToMinimunDiameterOfBasilarArtery() { setCurrentMode(MINIMUM_DIAMETER_OF_BASILAR_ARTERY); }
	virtual void setCurrentModeToMinimumDiameterOfLeftVertebralArtery() { setCurrentMode(MINIMUM_DIAMETER_OF_LEFT_VERTEBRAL_ARTERY); }
	virtual void setCurrentModeToMinimumDiameterOfRightVertebralArtery() { setCurrentMode(MINIMUM_DIAMETER_OF_RIGHT_VERTEBRAL_ARTERY); }
	virtual void FindMaximumRadius() override;
	virtual void FindMaxiMumPerpendicularDistance();

protected:
	QInteractorStyleSurfaceCenterLineVBDUboguStandard(int uiType = MULTIPLE_UI, QWidget* parent = Q_NULLPTR);
	virtual ~QInteractorStyleSurfaceCenterLineVBDUboguStandard() override;

	virtual void uniqueDisable() override {};
	virtual void uniqueEnable() override {};
	virtual void uniqueCall() override {};

	virtual void InitializeLinePerpendicularMeasurementWidget();
	void initialization();
	int m_currentMode = 0;

	vtkSmartPointer<PerpendicularMeasurementLineWidget> m_lineWidget = nullptr;

private:
	Ui::QInteractorStyleSurfaceCenterLineVBDUboguStandard* ui = nullptr;
};

#endif // !__QINTERACTOR_STYLE_SURFACE_CENTERLINE_VBD_UBOGU_STANDARD_H__
