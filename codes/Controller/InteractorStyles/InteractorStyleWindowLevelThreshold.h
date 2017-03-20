#ifndef __INTERACTOR_STYLE_WINDOW_LEVEL_THRESHOLD_H__
#define __INTERACTOR_STYLE_WINDOW_LEVEL_THRESHOLD_H__

#include "InteractorStyleWindowLevel.h"

class InteractorStyleWindowLevelThreshold : public InteractorStyleWindowLevel
{
public:
	vtkTypeMacro(InteractorStyleWindowLevelThreshold, InteractorStyleWindowLevel);
	static InteractorStyleWindowLevelThreshold* New();

	virtual void SetOutputLabel(int label);
	virtual void ThresholdToOverlay();

protected: 

	unsigned char m_label = 1;


};


#endif // !__INTERACTOR_STYLE_WINDOW_LEVEL_THRESHOLD_H__
