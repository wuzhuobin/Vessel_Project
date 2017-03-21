#include "ui_QInteractorStyleMaximumWallThickness.h"
#include "ui_QAbstractNavigation.h"

#include "ImageViewer.h"
#include "QInteractorStyleMaximumWallThickness.h"
#include "MaximumWallThickness.h"

#include <vtkObjectFactory.h>
#include <vtkRenderer.h>
#include <vtkExtractVOI.h>
#include <vtkImageData.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkStringArray.h>
#include <vtkLine.h>
#include <vtkPointData.h>
#include <vtkPointSetToLabelHierarchy.h>
#include <vtkLabelPlacementMapper.h>
#include <vtkActor2D.h>
#include <vtkPolyDataMapper.h>


vtkStandardNewMacro(QInteractorStyleMaximumWallThickness);
QSETUP_UI_SRC(QInteractorStyleMaximumWallThickness);

//void QInteractorStyleMaximumWallThickness::uniqueInvoke(bool flag)
//{
//	QAbstractNavigation::uniqueInvoke(flag);
//	if (flag && flag != initializationFlag) {
//		// turn on codes
//
//		connect(QAbstractNavigation::getUi()->sliceSpinBoxX, SIGNAL(valueChanged(int)),
//			this, SLOT(slotChangeSlice()),
//			static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
//		connect(QAbstractNavigation::getUi()->sliceSpinBoxY, SIGNAL(valueChanged(int)),
//			this, SLOT(slotChangeSlice()),
//			static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
//		connect(QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
//			this, SLOT(slotChangeSlice()),
//			static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
//	}
//	// turn off
//	if (!flag && flag != initializationFlag) {
//		// turn off codes
//		disconnect(QAbstractNavigation::getUi()->sliceSpinBoxX, SIGNAL(valueChanged(int)),
//			this, SLOT(slotChangeSlice()));
//		disconnect(QAbstractNavigation::getUi()->sliceSpinBoxY, SIGNAL(valueChanged(int)),
//			this, SLOT(slotChangeSlice()));
//		disconnect(QAbstractNavigation::getUi()->sliceSpinBoxZ, SIGNAL(valueChanged(int)),
//			this, SLOT(slotChangeSlice()));
//
//	}
//	if (flag != initializationFlag) {
//
//	}
//	initializationFlag = flag;
//}

void QInteractorStyleMaximumWallThickness::SetCustomEnabled(bool flag)
{
	QInteractorStyleRuler::SetCustomEnabled(flag);
	uniqueInvoke(flag);
	if (flag ) {
		EnableMaximumWallThickneesLabel(m_MaximumWallThickneesLabelFlag);

	}
	else {

	}
	GetImageViewer()->Render();
}

void QInteractorStyleMaximumWallThickness::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	QInteractorStyleRuler::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	//QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
		UpdateMaximumWallThicknessLabel();
}

#include <vtkImageActor.h>

void QInteractorStyleMaximumWallThickness::EnableMaximumWallThickneesLabel(bool flag) {
	//InteractorStyleRuler::EnableMaximumWallThickneesLabel(flag);
	m_MaximumWallThickneesLabelFlag = flag;
	GetImageViewer()->GetImageActor()->SetVisibility(!flag);
	GetImageViewer()->GetOverlayActor()->SetVisibility(!flag);
		UpdateMaximumWallThicknessLabel();
}

void QInteractorStyleMaximumWallThickness::UpdateMaximumWallThicknessLabel()
{
	if(m_MaximumWallThickneesLabelFlag && 
		GetSliceOrientation() == vtkImageViewer2::SLICE_ORIENTATION_XY){


		int voi[6];
		QString errorText = "Normal";
		double distance = 0;

		GetImageViewer()->GetDisplayExtent(voi);
		voi[4] = voi[5] = GetSlice();
		vtkSmartPointer<vtkExtractVOI> extractVOI =
			vtkSmartPointer<vtkExtractVOI>::New();
		extractVOI->SetInputData(GetImageViewer()->GetOverlay());
		extractVOI->SetVOI(voi);
		extractVOI->Update();

		vtkSmartPointer<MaximumWallThickness> mwt =
			vtkSmartPointer<MaximumWallThickness>::New();
		mwt->SetSliceImage(extractVOI->GetOutput());
		mwt->SetLumemIntensity(2);
		mwt->SetVesselIntensity(1);
		try {
			mwt->Update();
			distance = mwt->GetDistanceLoopPairVect().at(0).Distance;
		}
		catch (MaximumWallThickness::ERROR_CODE e) {
			cerr << "MaximumWallThickness error: " << e << endl;
			switch (e) {
			case MaximumWallThickness::ERROR_EXTRACT_LOOP:
				errorText = "Extract loop failed!";
				break;
			case MaximumWallThickness::ERROR_EXTRACT_SLICE:
				errorText = "Extract slice failed!";
				break;
			case MaximumWallThickness::ERROR_INPUT_NOT_A_SLICE:
				errorText = "Input is not a slice!";
				break;
			case MaximumWallThickness::ERROR_THICKNESS_CAL:
				errorText = "";
				break;
			case MaximumWallThickness::ERROR_UNDEFINED_BRANCH:
				errorText = "Cannot pair up vessel wall and lumen!";
				break;
			case MaximumWallThickness::ERROR_VALUE_TRANSFORM:
				errorText = "Type casting failed!";
				break;
			default:
				break;
			}
			ui->plainTextEditError->setPlainText(errorText);
			if (GetImageViewer()->GetRenderer()->HasViewProp(this->m_lineActor)) {
				GetImageViewer()->GetRenderer()->RemoveActor(this->m_lineActor);
				//m_lineActor = nullptr;

			}
			if (GetImageViewer()->GetRenderer()->HasViewProp(this->m_labelActor)) {
				GetImageViewer()->GetRenderer()->RemoveActor(this->m_labelActor);
				//m_labelActor = nullptr;
			}
			return;
		}
		ui->plainTextEditError->setPlainText(errorText);
			vtkSmartPointer<vtkPolyData>::New();
		vtkSmartPointer<vtkPoints> _points =
			vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkCellArray> _lines =
			vtkSmartPointer<vtkCellArray>::New();

		// Add label array.
		vtkSmartPointer<vtkStringArray> labels =
			vtkSmartPointer<vtkStringArray>::New();
		labels->SetName("labels");
		// Add priority array.
		//vtkSmartPointer<vtkIntArray> sizes =
		//	vtkSmartPointer<vtkIntArray>::New();
		//sizes->SetName("sizes");
		//labels->Reset();

		std::vector<MaximumWallThickness::DistanceLoopPair> looppairs = mwt->GetDistanceLoopPairVect();
		for (int i = 0; i < looppairs.size(); i++)
		{
			MaximumWallThickness::DistanceLoopPair l_lp = looppairs.at(i);
			//double p1[3], p2[3];
			if (l_lp.LoopPair.first->GetNumberOfPoints() == 0) {
				//	// #DisplayErrorMsgHere
				//	this->ui->MWTTextBrowser->setText("Loop pair extraction incorrect!");
				//	for (int j = 0; j < 3; i++)
				//	{
				//		if (m_mainWnd->GetMyImageViewer(j)->GetSliceOrientation() == 2) {
				//			m_mainWnd->GetMyImageViewer(j)->GetAnnotationRenderer()->RemoveActor(this->m_lineActor);
				//			m_mainWnd->GetMyImageViewer(j)->GetAnnotationRenderer()->RemoveActor2D(this->m_labelActor);
				//		}
				//	}
				//	m_mainWnd->RenderAll2DViewers();
				if (GetImageViewer()->GetRenderer()->HasViewProp(this->m_lineActor)) {
					GetImageViewer()->GetRenderer()->RemoveActor(this->m_lineActor);
					//m_lineActor = nullptr;

				}
				if (GetImageViewer()->GetRenderer()->HasViewProp(this->m_labelActor)) {
					GetImageViewer()->GetRenderer()->RemoveActor(this->m_labelActor);
					//m_labelActor = nullptr;
				}
				return;
			}


			_points->InsertNextPoint(l_lp.LoopPair.first->GetPoint(l_lp.PIDs.first));
			_points->InsertNextPoint(l_lp.LoopPair.second->GetPoint(l_lp.PIDs.second));

			//vtkSmartPointer<vtkLine> line =
			//	vtkSmartPointer<vtkLine>::New();
			//line->GetPointIds()->SetId(i*2, i*2); // the second 0 is the index of the Origin in linesPolyData's points
			//line->GetPointIds()->SetId(i*2+1, i*2+1); // the second 1 is the index of P0 in linesPolyData's points

			//_lines->InsertNextCell(line);
			_lines->InsertCellPoint(_points->GetNumberOfPoints() - 2);
			_lines->InsertCellPoint(_points->GetNumberOfPoints() - 1);
			// Update UI
			char distanceString[25];
			sprintf_s(distanceString, "%.3f mm", l_lp.Distance);
			labels->InsertNextValue(distanceString);
			labels->InsertNextValue(" ");

			//sizes->InsertNextValue(3);
			//sizes->InsertNextValue(3);

		}

		vtkSmartPointer<vtkPolyData> thicknessLines =
			vtkSmartPointer<vtkPolyData>::New();
		thicknessLines->SetPoints(_points);
		thicknessLines->SetLines(_lines);
		thicknessLines->GetPointData()->AddArray(labels);
		//thicknessLines->GetPointData()->AddArray(sizes);
		
		vtkSmartPointer<vtkPolyDataMapper> _mapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		_mapper->SetInputData(thicknessLines);

		this->m_lineActor->SetMapper(_mapper);

		vtkSmartPointer<vtkPointSetToLabelHierarchy> pointSetToLabelHierarchyFilter =
			vtkSmartPointer<vtkPointSetToLabelHierarchy>::New();
		pointSetToLabelHierarchyFilter->SetInputData(
			thicknessLines);
		pointSetToLabelHierarchyFilter->SetLabelArrayName("labels");
		//pointSetToLabelHierarchyFilter->SetPriorityArrayName("sizes");
		pointSetToLabelHierarchyFilter->Update();

		// Create a mapper and actor for the labels.
		vtkSmartPointer<vtkLabelPlacementMapper> labelMapper =
			vtkSmartPointer<vtkLabelPlacementMapper>::New();
		labelMapper->SetInputConnection(
			pointSetToLabelHierarchyFilter->GetOutputPort());

		vtkSmartPointer<vtkActor2D> labelActor =
			vtkSmartPointer<vtkActor2D>::New();
		this->m_labelActor->SetMapper(labelMapper);

		if (!GetImageViewer()->GetRenderer()->HasViewProp(this->m_lineActor)) {
			GetImageViewer()->GetRenderer()->AddActor(this->m_lineActor);

		}
		if (!GetImageViewer()->GetRenderer()->HasViewProp(this->m_labelActor)) {
			GetImageViewer()->GetRenderer()->AddActor2D(this->m_labelActor);
		}
	}
	GetImageViewer()->Render();

}

QInteractorStyleMaximumWallThickness::QInteractorStyleMaximumWallThickness(int uiType, QWidget * parent)
	:QInteractorStyleRuler(NO_UI, this)
{
	QNEW_UI();
}

QInteractorStyleMaximumWallThickness::~QInteractorStyleMaximumWallThickness()
{
	QDELETE_UI();
}

void QInteractorStyleMaximumWallThickness::initialization()
{
	connect(ui->checkBoxMaximumWallThickness, SIGNAL(toggled(bool)),
		this, SLOT(EnableMaximumWallThickneesLabel(bool)));

	m_lineActor = vtkSmartPointer<vtkActor>::New();
	m_labelActor = vtkSmartPointer<vtkActor2D>::New();
}
