#include "InteractorStyleWindowLevelThreshold.h"

#include "ImageViewer.h"

#include <vtkLookupTable.h>
#include <vtkImageThreshold.h>
#include <vtkSmartPointer.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImageData.h>
#include <vtkObjectFactory.h>

vtkStandardNewMacro(InteractorStyleWindowLevelThreshold);

void InteractorStyleWindowLevelThreshold::SetOutputLabel(int label)
{
	vtkLookupTable* lookupTable = GetImageViewer()->GetLookupTable();
	if (label >= lookupTable->GetNumberOfTableValues()) {
		return;
	}
	m_label = label;
}

void InteractorStyleWindowLevelThreshold::ThresholdToOverlay()
{
	vtkSmartPointer<vtkImageThreshold> threshold =
		vtkSmartPointer<vtkImageThreshold>::New();
	threshold->SetNumberOfThreads(16);
	threshold->SetInputData(GetImageViewer()->GetInput());
	threshold->SetOutputScalarType(GetImageViewer()->GetOverlay()->GetScalarType());
	threshold->ThresholdByUpper(GetLevel()- GetWindow() * 0.5);
	threshold->ReplaceOutOn();
	threshold->SetOutValue(0);
	threshold->ReplaceInOn();
	threshold->SetInValue(m_label);
	threshold->Update();
	GetImageViewer()->GetOverlay()->ShallowCopy(threshold->GetOutput());
	SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(InteractorStyleWindowLevelThreshold, GetImageViewer()->Render());
}

