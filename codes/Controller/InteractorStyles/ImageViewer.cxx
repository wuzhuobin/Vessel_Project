/*=========================================================================

Program:   Visualization Toolkit
Module:    ImageViewer.cxx

Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
All rights reserved.
See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "ImageViewer.h"

#include <vtkInformation.h>
#include <vtkCamera.h>
#include <vtkImageProperty.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkCachedStreamingDemandDrivenPipeline.h>
#include <vtkImageMapper3D.h>
#include <vtkTextProperty.h>
#include <vtkProperty2D.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkCallbackCommand.h>

#include <vtkTextActor.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkLogLookupTable.h>
#include <vtkImageActor.h>
#include <vtkCursor3D.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkImageData.h>
#include <vtkExtractVOI.h>
#include <vtkObjectFactory.h>

#include <algorithm>


//#include <QList>
using std::string;
using std::min;
using std::max;
vtkStandardNewMacro(ImageViewer);

//----------------------------------------------------------------------------
ImageViewer::ImageViewer(/*QObject* parent*/)
	//:QObject(parent)
{
	this->OverlayActor = vtkImageActor::New();
	this->OverlayWindowLevel = vtkImageMapToWindowLevelColors::New();

	// OrientationTextActor 
	this->IntTextActor = vtkTextActor::New();
	this->HeaderActor = vtkTextActor::New();
	for (int i = 0; i < 4; i++)
	{
		this->OrientationTextActor[i] = vtkTextActor::New();
	}

	//Cursor
	Cursor3D = vtkCursor3D::New();
	Cursor3D->SetTranslationMode(false);
	Cursor3D->AllOff();
	Cursor3D->AxesOn();
	Cursor3D->SetModelBounds(0, 0, 0, 0, 0, 0);
	Cursor3D->Update();

	CursorMapper = vtkPolyDataMapper::New();

	CursorActor = vtkActor::New();
	CursorActor->GetProperty()->SetColor(0, 0, 1);
	CursorActor->GetProperty()->SetLineStipplePattern(0xf0f0);
	if (this->CursorActor && this->CursorMapper) {
		this->CursorActor->SetMapper(this->CursorMapper);
	}
	//AnnotationRenderer
	//AnnotationRenderer->SetLayer(1);

	//RenderWindow->SetNumberOfLayers(2);

	this->InstallPipeline();
	UpdateOrientation();
	//this->OverlayActor->SetVisibility(false);
	// Error blocking fot windowLevel
	// temporary fixation
	//vtkSmartPointer<vtkCallbackCommand> windowLevelErrorBlocker =
	//	vtkSmartPointer<vtkCallbackCommand>::New();
	//this->WindowLevel->GetExecutive()->AddObserver(
	//	vtkCommand::ErrorEvent, windowLevelErrorBlocker);
	//this->OverlayWindowLevel->GetExecutive()->AddObserver(
	//	vtkCommand::ErrorEvent, windowLevelErrorBlocker);
	//this->WindowLevel->GetInputAlgorithm()->AddObserver(
	//	vtkCommand::ErrorEvent, windowLevelErrorBlocker);
	//this->OverlayWindowLevel->GetInputAlgorithm()->AddObserver(
	//	vtkCommand::ErrorEvent, windowLevelErrorBlocker);
}

//----------------------------------------------------------------------------
ImageViewer::~ImageViewer()
{
	//if (this->ImageExtractVOI) {
	//	this->ImageExtractVOI->Delete();
	//	this->ImageExtractVOI = NULL;
	//}
	//if (this->OverlayExtractVOI) {
	//	this->OverlayExtractVOI->Delete();
	//	this->OverlayExtractVOI = NULL;
	//}
	if (this->OverlayWindowLevel) {
		this->OverlayWindowLevel->Delete();
		this->OverlayWindowLevel = NULL;
	}
	if (this->OverlayActor) {
		this->OverlayActor->Delete();
		this->OverlayActor = NULL;
	}
	//if (this->AnnotationRenderer != NULL) {
	//	this->AnnotationRenderer->Delete();
	//	this->AnnotationRenderer = NULL;
	//}
	// OrientationTextActor
	if (IntTextActor != NULL) {
		this->IntTextActor->Delete();
		this->IntTextActor = NULL;
	}
	if (HeaderActor != NULL) {
		this->HeaderActor->Delete();
		this->HeaderActor = NULL;
	}
	for (int i = 0; i < 4; i++)
	{
		if (OrientationTextActor[i] != NULL) {
			OrientationTextActor[i]->Delete();
			this->OrientationTextActor[i] = NULL;
		}
	}
	// Cursor
	if (this->Cursor3D != NULL) {
		this->Cursor3D->Delete();
		this->Cursor3D = NULL;
	}
	if (this->CursorMapper != NULL) {
		this->CursorMapper->Delete();
		this->CursorMapper = NULL;
	}
	if (this->CursorActor != NULL) {
		this->CursorActor->Delete();
		this->CursorActor = NULL;
	}

}

void ImageViewer::UpdateDisplayExtent(int * displayExtent)
{
	SetDisplayExtent(displayExtent);
	UpdateDisplayExtent();
}

//----------------------------------------------------------------------------
void ImageViewer::UpdateDisplayExtent()
{
	vtkAlgorithm *input = this->GetInputAlgorithm();
	if (!input || !this->ImageActor || !this->OverlayActor)
	{
		return;
	}

	input->UpdateInformation();
	vtkInformation* outInfo = input->GetOutputInformation(0);
	//int *DisplayExtent = outInfo->Get(
	//	vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT());
	//DisplayExtent[0] = max(DisplayExtent[0], DisplayExtent[0]);
	//DisplayExtent[2] = max(DisplayExtent[2], DisplayExtent[2]);
	//DisplayExtent[4] = max(DisplayExtent[4], DisplayExtent[4]);
	//DisplayExtent[1] = min(DisplayExtent[1], DisplayExtent[1]);
	//DisplayExtent[3] = min(DisplayExtent[3], DisplayExtent[3]);
	//DisplayExtent[5] = min(DisplayExtent[5], DisplayExtent[5]);
	//if (FirstRender) {
	//	memcpy(DisplayExtent, DisplayExtent, sizeof(DisplayExtent));
	//}
	//else {
	//	memcpy(DisplayExtent, DisplayExtent, sizeof(DisplayExtent));
	//}

	// Is the slice in range ? If not, fix it

	int slice_min = DisplayExtent[this->SliceOrientation * 2];
	int slice_max = DisplayExtent[this->SliceOrientation * 2 + 1];
	if (this->Slice < slice_min || this->Slice > slice_max)
	{
		this->Slice = static_cast<int>((slice_min + slice_max) * 0.5);
	}

	// Set the image actor

	switch (this->SliceOrientation)
	{
	case vtkImageViewer2::SLICE_ORIENTATION_XY:
		this->ImageActor->SetDisplayExtent(
			DisplayExtent[0], DisplayExtent[1], DisplayExtent[2], DisplayExtent[3], this->Slice, this->Slice);
		this->OverlayActor->SetDisplayExtent(
			DisplayExtent[0], DisplayExtent[1], DisplayExtent[2], DisplayExtent[3], this->Slice, this->Slice);
		break;

	case vtkImageViewer2::SLICE_ORIENTATION_XZ:
		this->ImageActor->SetDisplayExtent(
			DisplayExtent[0], DisplayExtent[1], this->Slice, this->Slice, DisplayExtent[4], DisplayExtent[5]);
		this->OverlayActor->SetDisplayExtent(
			DisplayExtent[0], DisplayExtent[1], this->Slice, this->Slice, DisplayExtent[4], DisplayExtent[5]);
		break;

	case vtkImageViewer2::SLICE_ORIENTATION_YZ:
		this->ImageActor->SetDisplayExtent(
			this->Slice, this->Slice, DisplayExtent[2], DisplayExtent[3], DisplayExtent[4], DisplayExtent[5]);
		this->OverlayActor->SetDisplayExtent(
			this->Slice, this->Slice, DisplayExtent[2], DisplayExtent[3], DisplayExtent[4], DisplayExtent[5]);
		break;
	}
	InitializeCursorBoundary();
	// Figure out the correct clipping range

	if (this->Renderer)
	{
		if (this->InteractorStyle &&
			this->InteractorStyle->GetAutoAdjustCameraClippingRange())
		{
			this->Renderer->ResetCameraClippingRange();
		}
		else
		{
			vtkCamera *cam = this->Renderer->GetActiveCamera();
			if (cam)
			{
				double bounds[6];
				this->ImageActor->GetBounds(bounds);
				double spos = bounds[this->SliceOrientation * 2];
				double cpos = cam->GetPosition()[this->SliceOrientation];
				double range = fabs(spos - cpos);
				double *spacing = outInfo->Get(vtkDataObject::SPACING());
				double avg_spacing =
					(spacing[0] + spacing[1] + spacing[2]) / 3.0;
				cam->SetClippingRange(
					range - avg_spacing * 3.0, range + avg_spacing * 3.0);
			}
		}
	}
}
void ImageViewer::ResetDisplayExtent()
{
	vtkAlgorithm *input = this->GetInputAlgorithm();
	if (!input || !this->ImageActor || !this->OverlayActor)
	{
		return;
	}

	input->UpdateInformation();
	vtkInformation* outInfo = input->GetOutputInformation(0);
	int *ResetExtent = outInfo->Get(
		vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT());
	memcpy(DisplayExtent, ResetExtent, sizeof(DisplayExtent));
	UpdateDisplayExtent();
}

void ImageViewer::SetColorLevel(double level)
{
	if (GetColorLevel() == level)
		return;
	vtkImageViewer2::SetColorLevel(level);
	//emit ColorLevelChanged(level);

}

void ImageViewer::SetColorWindow(double window)
{
	if (GetColorWindow() == window)
		return;
	vtkImageViewer2::SetColorWindow(window);
	//emit ColorWindowChanged(window);
}

//----------------------------------------------------------------------------
void ImageViewer::InstallPipeline()
{
	vtkImageViewer2::InstallPipeline();

	// add label view prop to renderer
	if (this->Renderer && this->OverlayActor) 
	{
		this->Renderer->AddViewProp(this->OverlayActor);
	}
	// Setup connection with window level mapper
	if (this->OverlayActor && this->OverlayWindowLevel)
	{
		this->OverlayActor->GetMapper()->SetInputConnection(
			this->OverlayWindowLevel->GetOutputPort());
	}
	// add cursor 
	if (this->CursorMapper && this->Cursor3D) {
		this->CursorMapper->SetInputConnection(this->Cursor3D->GetOutputPort());
	}

	if (this->Renderer && this->CursorActor)
	{
		this->Renderer->AddActor(this->CursorActor);
	}
	// Text 
	if (this->HeaderActor && this->Renderer) {
		this->Renderer->AddActor(this->HeaderActor);
	}
	if (this->IntTextActor && this->Renderer) {
		this->Renderer->AddActor(this->IntTextActor);
	}
	for (int i = 0; i < 4; ++i) {
		if (this->OrientationTextActor[i] && this->Renderer) {
			this->Renderer->AddActor(this->OrientationTextActor[i]);
		}
	}
}

//----------------------------------------------------------------------------
void ImageViewer::UnInstallPipeline()
{
	if (this->OverlayActor)
	{
		this->OverlayActor->GetMapper()->SetInputConnection(NULL);
	}
	if (this->Renderer && this->OverlayActor) {
		this->Renderer->RemoveActor(OverlayActor);
	}
	if (this->CursorMapper) {
		this->CursorMapper->SetInputConnection(nullptr);
	}
	if (this->Renderer && this->CursorActor)
	{
		this->Renderer->RemoveActor(this->CursorActor);
	}

	// Text 
	if (this->HeaderActor && this->Renderer) {
		this->Renderer->RemoveActor(this->HeaderActor);
	}
	if (this->IntTextActor && this->Renderer) {
		this->Renderer->RemoveActor(this->IntTextActor);
	}
	for (int i = 0; i < 4; ++i) {
		if (this->OrientationTextActor[i] && this->Renderer) {
			this->Renderer->RemoveActor(this->OrientationTextActor[i]);
		}
	}
	vtkImageViewer2::UnInstallPipeline();
}

//----------------------------------------------------------------------------
void ImageViewer::UpdateOrientation()
{
	// Set the camera position

	vtkCamera *cam = this->Renderer ? this->Renderer->GetActiveCamera() : NULL;
	if (cam)
	{
		switch (this->SliceOrientation)
		{
		case vtkImageViewer2::SLICE_ORIENTATION_XY:
			cam->SetFocalPoint(0, 0, 0);
			cam->SetPosition(0, 0, -1); // -1 if medical ?
			cam->SetViewUp(0, -1, 0);
			break;

		case vtkImageViewer2::SLICE_ORIENTATION_XZ:
			cam->SetFocalPoint(0, 0, 0);
			cam->SetPosition(0, -1, 0); // 1 if medical ?
			cam->SetViewUp(0, 0, 1);
			break;

		case vtkImageViewer2::SLICE_ORIENTATION_YZ:
			cam->SetFocalPoint(0, 0, 0);
			cam->SetPosition(1, 0, 0); // -1 if medical ?
			cam->SetViewUp(0, 0, 1);
			break;
		}
	}
}
//----------------------------------------------------------------------------
void ImageViewer::Render()
{
	if (this->FirstRender)
	{
		this->InitializeOrientationText();
		this->InitializeIntensityText("");
		this->InitializeHeader(string());
	}
	vtkImageViewer2::Render();

	// update orientation and header text
	ResizeHeaderAndOrientationText();
}
//----------------------------------------------------------------------------
void ImageViewer::SetInputData(vtkImageData *in)
{
	// when there is a new input, Update the member DisplayExtent 
	vtkImageViewer2::SetInputData(in);
	ResetDisplayExtent();
	//Color Map
	double* range = in->GetScalarRange();
	this->SetColorWindow(range[1] - range[0]);
	this->SetColorLevel((range[1] + range[0])*0.5);


	//DefaultWindowLevel[0] = this->GetColorWindow();
	//DefaultWindowLevel[1] = this->GetColorLevel();
}

void ImageViewer::SetInputDataLayer(vtkImageData *in)
{
	//OverlayExtractVOI->SetInputData(in);
	//OverlayExtractVOI->SetVOI(in->GetExtent());
	//OverlayExtractVOI->Update();

	OverlayWindowLevel->SetInputData(in);
	// in case when LookupTable has not been set
	//if (this->LookupTable != NULL) {
	//	int num = this->LookupTable->GetNumberOfTableValues();
	//	OverlayWindowLevel->SetWindow(num - 1 );
	//	OverlayWindowLevel->SetLevel((num - 1)*0.5);
	//}
	//this->UpdateDisplayExtent();

}
//----------------------------------------------------------------------------
vtkImageData* ImageViewer::GetInputLayer()
{
	return vtkImageData::SafeDownCast(this->OverlayWindowLevel->GetInput());
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ImageViewer::PrintSelf(ostream& os, vtkIndent indent)
{
	this->vtkImageViewer2::PrintSelf(os, indent);

	os << indent << "RenderWindow:\n";
	this->RenderWindow->PrintSelf(os, indent.GetNextIndent());
	os << indent << "Renderer:\n";
	this->Renderer->PrintSelf(os, indent.GetNextIndent());
	os << indent << "ImageActor:\n";
	this->ImageActor->PrintSelf(os, indent.GetNextIndent());
	os << indent << "ImageActorContour:\n";
	this->OverlayActor->PrintSelf(os, indent.GetNextIndent());
	os << indent << "WindowLevel:\n" << endl;
	this->WindowLevel->PrintSelf(os, indent.GetNextIndent());
	os << indent << "Slice: " << this->Slice << endl;
	os << indent << "SliceOrientation: " << this->SliceOrientation << endl;
	os << indent << "InteractorStyle: " << endl;
	if (this->InteractorStyle)
	{
		os << "\n";
		this->InteractorStyle->PrintSelf(os, indent.GetNextIndent());
	}
	else
	{
		os << "None";
	}
}

void ImageViewer::InitializeCursorBoundary()
{
	// uncomment the following code will lead the cursor boundary updated normally
	// but it will lead to the canvas source behaviour very strange.

	//this->GetInputAlgorithm()->Update();
	//const double* spacing = GetInput()->GetSpacing();
	//const double* origin = GetInput()->GetOrigin();
	//const int* extent = GetInput()->GetExtent();
	//double bound[6];
	//bound[0] = origin[0] + extent[0] * spacing[0];
	//bound[1] = origin[0] + extent[1] * spacing[0];
	//bound[2] = origin[1] + extent[2] * spacing[1];
	//bound[3] = origin[1] + extent[3] * spacing[1];
	//bound[4] = origin[2] + extent[4] * spacing[2];
	//bound[5] = origin[2] + extent[5] * spacing[2];

	//Cursor3D->SetModelBounds(bound);
	Cursor3D->SetModelBounds(GetInput()->GetBounds());
	Cursor3D->Update();
}

//void ImageViewer::SetAnnotationRenderer(vtkRenderer * arg)
//{
//	if (this->AnnotationRenderer == arg) {
//		return;
//	}
//	this->UnInstallPipeline();
//
//	if (this->AnnotationRenderer)
//	{
//		this->AnnotationRenderer->UnRegister(this);
//	}
//
//	this->AnnotationRenderer = arg;
//
//	if (this->AnnotationRenderer)
//	{
//		this->AnnotationRenderer->Register(this);
//	}
//
//	this->InstallPipeline();
//	this->UpdateOrientation();
//}

vtkLookupTable* ImageViewer::GetLookupTable()
{
	return this->LookupTable;
}

void ImageViewer::SetLookupTable(vtkLookupTable * LookupTable)
{
	this->LookupTable = LookupTable;
	this->OverlayActor->GetProperty()->SetInterpolationTypeToNearest();
	this->OverlayActor->GetProperty()->SetLookupTable(LookupTable);

	int num = LookupTable->GetNumberOfTableValues();
	OverlayWindowLevel->SetWindow(num - 1);
	OverlayWindowLevel->SetLevel((num - 1) * 0.5);
}

void ImageViewer::SetFocalPointWithWorldCoordinate(double x, double y, double z)
{
	const double* spacing = GetInput()->GetSpacing();
	const double* origin = GetInput()->GetOrigin();
	int i = (x - origin[0])/spacing[0];
	int j = (y - origin[1])/spacing[1];
	int k = (z - origin[2])/spacing[2];
	SetFocalPointWithImageCoordinate(i, j, k);

}

void ImageViewer::SetFocalPointWithImageCoordinate(int i, int j, int k)
{
	const double* spacing = GetInput()->GetSpacing();
	const double* origin = GetInput()->GetOrigin();
	const int* extent = GetInput()->GetExtent();
	if (i < extent[0] || i > extent[1] || 
		j < extent[2] || j > extent[3] || 
		k < extent[4] || k > extent[5]
		) {
		return;
	}
	double point[3] = { 
		i*spacing[0] + origin[0],
		j*spacing[1] + origin[1], 
		k*spacing[2] + origin[2] };

	const double* pointOld = Cursor3D->GetFocalPoint();

	if (pointOld[0] == point[0] && pointOld[1] == point[1] && pointOld[2] == point[2]) {
		return;
	}
	

	Cursor3D->SetFocalPoint(point);
	Cursor3D->Update();
	int ijk[3] = { i, j, k };
	SetSlice(ijk[this->SliceOrientation]);
	InitializeIntensityText(std::to_string(
		(GetInput()->GetScalarComponentAsDouble(i, j, k, 0))));
	this->Render();

	//emit FocalPointWithImageCoordinateChanged(i, j, k);
}

void ImageViewer::GetFocalPointWithImageCoordinate(int * coordinate)
{
	const double* spacing = GetInput()->GetSpacing();
	const double* origin = GetInput()->GetOrigin();
	const double* point = Cursor3D->GetFocalPoint();
	for (int i = 0; i < 3; ++i) {
		coordinate[i] = (point[i] - origin[i]) / spacing[i];
	}
}

void ImageViewer::GetFocalPointWithWorldCoordinate(double * coordinate)
{
	memcpy(coordinate, Cursor3D->GetFocalPoint(), sizeof(double) * 3);
}

double * ImageViewer::GetFocalPointWithWorldCoordinate()
{
	return Cursor3D->GetFocalPoint();
}

double * ImageViewer::GetCursorBoundWithWorldCoordinate()
{
	return this->Cursor3D->GetModelBounds();
}

void ImageViewer::SetAllBlack(bool flag)
{
	if (this->AllBlackFlag == flag) {
		return;
	}
	this->AllBlackFlag = flag;
	//QList<vtkProp*>  props;
	//props += this->ImageActor;
	//props += this->OrientationTextActor[0];
	//props += this->OrientationTextActor[1];
	//props += this->OrientationTextActor[2];
	//props += this->OrientationTextActor[3];
	//props += this->HeaderActor;
	//props += this->IntTextActor;
	//props += this->CursorActor;
	//props += this->OverlayActor;


	//for (QList<vtkProp*>::const_iterator cit = props.cbegin(); cit != props.cend(); ++cit) {
	//	if ((*cit) != NULL) {
	//		(*cit)->SetVisibility(!flag);
	//	}
	//}
	//Render();
	//emit AllBlackAlready(flag);
}

bool ImageViewer::GetAllBlack()
{
	return AllBlackFlag;
}

//double* ImageViewer::GetDefaultWindowLevel()
//{
//	return DefaultWindowLevel;
//}

void ImageViewer::InitializeHeader(string file)
{

	const int* size = Renderer->GetSize();
	const int margin = 15;
	int coord[2] = { 5,size[1] - margin };
	if (this->FirstRender && HeaderActor != NULL) {
		HeaderActor->SetDisplayPosition(coord[0], coord[1]);
		HeaderActor->GetTextProperty()->SetFontSize(15);
		HeaderActor->GetTextProperty()->SetColor(1.0, 0.7490, 0.0);
		Renderer->AddActor2D(HeaderActor);
	}

	if (GetInput() != NULL)
		HeaderActor->SetInput(file.c_str());
	else {
		HeaderActor->SetInput("");
		cout << "Error in setting text, file not found" << endl;
	}
}

//void ImageViewer::InitializeHeader(QString file)
//{
//	InitializeHeader(file.toStdString());
//}

void ImageViewer::InitializeIntensityText(string IntText)
{
	const int* size = Renderer->GetSize();
	const int coord[2] = { 5,5 };
	if (this->FirstRender && IntTextActor != NULL) {
		IntTextActor->SetDisplayPosition(coord[0], coord[1]);
		IntTextActor->GetTextProperty()->SetFontSize(15);
		IntTextActor->GetTextProperty()->SetColor(1.0, 0.7490, 0.0);
		Renderer->AddActor2D(IntTextActor);
		return;
	}

	if (GetInput() != NULL)
		IntTextActor->SetInput(IntText.c_str());
	else {
		IntTextActor->SetInput("");
		cout << "Error in setting text, file not found" << endl;
	}

}

void ImageViewer::InitializeOrientationText()
{
	int* size = Renderer->GetSize();
	int margin = 15;

	int down[2] = { size[0] / 2	,5 };
	int up[2] = { size[0] / 2	,size[1] - margin };
	int left[2] = { 5			,size[1] / 2 };
	int right[2] = { size[0] - margin	,size[1] / 2 };
	int* position[4] = { up, down, left, right };

	for (int i = 0; i<4; i++)
	{
		if (this->FirstRender && OrientationTextActor[i] != NULL) {
			OrientationTextActor[i]->SetDisplayPosition(position[i][0], position[i][1]);
			OrientationTextActor[i]->GetTextProperty()->SetFontSize(15);
			OrientationTextActor[i]->GetTextProperty()->SetColor(1.0, 0.7490, 0.0);
			Renderer->AddActor2D(OrientationTextActor[i]);
		}
	}
	switch (SliceOrientation)
	{
	case 0:
		OrientationTextActor[0]->SetInput("S");
		OrientationTextActor[1]->SetInput("I");
		OrientationTextActor[2]->SetInput("A");
		OrientationTextActor[3]->SetInput("P");
		break;
	case 1:
		OrientationTextActor[0]->SetInput("S");
		OrientationTextActor[1]->SetInput("I");
		OrientationTextActor[2]->SetInput("R");
		OrientationTextActor[3]->SetInput("L");
		break;
	case 2:
		OrientationTextActor[0]->SetInput("A");
		OrientationTextActor[1]->SetInput("P");
		OrientationTextActor[2]->SetInput("R");
		OrientationTextActor[3]->SetInput("L");
		break;
	}

}

void ImageViewer::ResizeHeaderAndOrientationText()
{
	int* size = Renderer->GetSize();
	int margin = 15;
	int coord[2] = { 5,size[1] - margin };
	int down[2] = { size[0] / 2	,5 };
	int up[2] = { size[0] / 2	,size[1] - margin };
	int left[2] = { 5			,size[1] / 2 };
	int right[2] = { size[0] - margin	,size[1] / 2 };
	int* position[4] = { up, down, left, right };

	for (int i = 0; i<4; i++)
	{
		OrientationTextActor[i]->SetDisplayPosition(position[i][0], position[i][1]);
	}
	HeaderActor->SetDisplayPosition(coord[0], coord[1]);
}

void ImageViewer::SetSlice(int s)
{
	if (Slice == s)
		return;
	vtkImageViewer2::SetSlice(s);
	//emit SliceChanged(s);
}

void ImageViewer::SetupInteractor(vtkRenderWindowInteractor * arg)
{
	vtkImageViewer2::SetupInteractor(arg);
	//if (this->AnnotationRenderer) {
	//	this->AnnotationRenderer->SetActiveCamera(this->Renderer->GetActiveCamera());
	//}
}
