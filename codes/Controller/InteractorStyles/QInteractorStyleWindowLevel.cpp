﻿#include "QInteractorStyleWindowLevel.h"
#include "ui_qinteractorstylewindowlevel.h"
#include "ui_QAbstractNavigation.h"

#include <vtkObjectFactory.h>

vtkStandardNewMacro(QInteractorStyleWindowLevel);
QSETUP_UI_SRC(QInteractorStyleWindowLevel);

QInteractorStyleWindowLevel::QInteractorStyleWindowLevel(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleWindowLevel::~QInteractorStyleWindowLevel()
{
	QDELETE_UI();
}

void QInteractorStyleWindowLevel::SetCustomEnabled(bool flag)
{
	InteractorStyleWindowLevel::SetCustomEnabled(flag);
	uniqueInvoke(flag);
}

void QInteractorStyleWindowLevel::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleWindowLevel::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}