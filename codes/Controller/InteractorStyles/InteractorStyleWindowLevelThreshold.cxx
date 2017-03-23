#include "InteractorStyleWindowLevelThreshold.h"

#include "ImageViewer.h"

#include <vtkLookupTable.h>
#include <vtkImageThreshold.h>
#include <vtkSmartPointer.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImageData.h>
#include <vtkObjectFactory.h>
#include <vtkImageBlend.h>
#include <vtkExtractVOI.h>

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
	vtkSmartPointer<vtkExtractVOI> extractVOI =
		vtkSmartPointer<vtkExtractVOI>::New();
	extractVOI->SetInputData(GetImageViewer()->GetInput());
	extractVOI->SetVOI(GetImageViewer()->GetDisplayExtent());
	extractVOI->Update();



	vtkSmartPointer<vtkImageThreshold> threshold =
		vtkSmartPointer<vtkImageThreshold>::New();
	threshold->SetNumberOfThreads(16);
	threshold->SetInputConnection(extractVOI->GetOutputPort());
	//threshold->SetInputData(GetImageViewer()->GetInput());
	threshold->SetOutputScalarType(GetImageViewer()->GetOverlay()->GetScalarType());
	threshold->ThresholdByUpper(GetLevel()- GetWindow() * 0.5);
	threshold->ReplaceOutOn();
	threshold->SetOutValue(0);
	threshold->ReplaceInOn();
	threshold->SetInValue(m_label);
	threshold->Update();

	vtkSmartPointer<vtkImageBlend> imageBlend =
		vtkSmartPointer<vtkImageBlend>::New();
	imageBlend->AddInputData(GetImageViewer()->GetOverlay());
	imageBlend->AddInputConnection(threshold->GetOutputPort());
	imageBlend->SetOpacity(1, 1);
	imageBlend->Update();

	GetImageViewer()->GetOverlay()->ShallowCopy(imageBlend->GetOutput());
	SAFE_DOWN_CAST_IMAGE_CONSTITERATOR(InteractorStyleWindowLevelThreshold, GetImageViewer()->Render());
}

