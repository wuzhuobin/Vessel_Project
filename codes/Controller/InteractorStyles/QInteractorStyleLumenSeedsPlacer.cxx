#include "QInteractorStyleLumenSeedsPlacer.h"

#include <vtkRenderWindowInteractor.h>
#include <vtkMarchingSquares.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkObjectFactory.h>
#include <vtkImageData.h>
#include <vtkExtractVOI.h>
#include <vtkSeedWidget.h>
#include <vtkParametricSpline.h>
#include <vtkParametricFunctionSource.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTransform.h>
#include <vtkImageMask.h>

#include "vtkPolylineToTubularVolume.h"
#include "LumenExtractionFilter.h"
#include "ImageViewer.h"
#include "ui_QInteractorStyleLumenSeedsPlacer.h"
#include "ui_QAbstractNavigation.h"


vtkStandardNewMacro(QInteractorStyleLumenSeedsPlacer);
QSETUP_UI_SRC(QInteractorStyleLumenSeedsPlacer);
using namespace std;

void QInteractorStyleLumenSeedsPlacer::SetCustomEnabled(bool flag)
{
	InteractorStyleSeedsPlacer::SetCustomEnabled(flag);
	uniqueInvoke(flag);
}

void QInteractorStyleLumenSeedsPlacer::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleSeedsPlacer::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStyleLumenSeedsPlacer::SetTargetImages(
	QList<vtkSmartPointer<vtkImageData>> listOfVtkImages, 
	QList<QString> listOfModalityNames)
{
	m_listOfModalityNames = listOfModalityNames;
	m_listOfVtkImages = listOfVtkImages;

	ui->comboBoxTargeImage->clear();
	for (int i = 0; i < m_listOfModalityNames.size(); ++i) {
		if (m_listOfVtkImages[i] != nullptr) {
			ui->comboBoxTargeImage->addItem(m_listOfModalityNames[i]);
		}
	}
}

void QInteractorStyleLumenSeedsPlacer::UpdateWidgetToSeeds(int * newImagePos, int* oldImagePos)
{
	InteractorStyleSeedsPlacer::UpdateWidgetToSeeds(newImagePos, oldImagePos);
	ui->listWidgetSeedList->clear();
	for (list<int*>::const_iterator cit = m_seeds.cbegin(); cit != m_seeds.cend(); ++cit) {
		QString listItem = "Seed Index: [" +
			QString::number((*cit)[0]) + "," +
			QString::number((*cit)[1]) + "," +
			QString::number((*cit)[2]) + "]";
		ui->listWidgetSeedList->addItem(listItem);
	}
}

void QInteractorStyleLumenSeedsPlacer::SlotClearAllSeeds()
{
	ClearAllSeeds();
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, ClearAllSeedWidget());
	ui->listWidgetSeedList->clear();
	m_seedWidget->Render();
}

void QInteractorStyleLumenSeedsPlacer::SetFocalSeed(int i)
{
	InteractorStyleSeedsPlacer::SetFocalSeed(i);
	const double* worldCoordinate = GetImageViewer()->GetFocalPointWithWorldCoordinate();

	//char label[60];
	//sprintf_s(label, "(%.3f, %.3f, %.3f)", worldCoordinate[0], worldCoordinate[1], worldCoordinate[2]);
	QString label = "(" + QString::number(worldCoordinate[0], 'f', 3) + "," +
		QString::number(worldCoordinate[1], 'f', 3) + "," +
		QString::number(worldCoordinate[2], 'f', 3) + ")";
	/// change label text
	this->ui->labelSeedCoordinate->setText(label);
}

void QInteractorStyleLumenSeedsPlacer::DeleteFocalSeed()
{
	QList<int*> _seeds = QList<int*>::fromStdList(m_seeds);
	int i = ui->listWidgetSeedList->currentRow();
	if (i >= 0 && i < _seeds.size()) {
		ui->listWidgetSeedList->removeItemWidget(
			ui->listWidgetSeedList->takeItem(i));
		m_seeds.remove(_seeds[i]);
		delete _seeds[i];
	}
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, ClearAllSeedWidget());
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, GenerateWidgetFromSeeds());
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, m_seedWidget->Render());
}

void QInteractorStyleLumenSeedsPlacer::SaveWidgetToSeeds(std::list<int*>& seed)
{
	InteractorStyleSeedsPlacer::SaveWidgetToSeeds(seed);
	ui->listWidgetSeedList->clear();
	for (list<int*>::const_iterator cit = m_seeds.cbegin(); cit != m_seeds.cend(); ++cit) {
		QString listItem = "Seed Index: [" +
			QString::number((*cit)[0]) + "," +
			QString::number((*cit)[1]) + "," +
			QString::number((*cit)[2]) + "]";
		ui->listWidgetSeedList->addItem(listItem);
	}
}

void QInteractorStyleLumenSeedsPlacer::DropSeed()
{
	InteractorStyleSeedsPlacer::DropSeed();
}

void QInteractorStyleLumenSeedsPlacer::ExtractLumen()
{
	typedef itk::Index<3> IndexType;
	int index = m_listOfModalityNames.indexOf(ui->comboBoxTargeImage->currentText());

	vtkSmartPointer<vtkExtractVOI> extractVOI =
		vtkSmartPointer<vtkExtractVOI>::New();
	extractVOI->SetInputData(GetImageViewer()->GetInput());
	extractVOI->SetVOI(GetImageViewer()->GetDisplayExtent());
	extractVOI->Update();

	vtkSmartPointer<LumenExtractionFilter> lumenExtractionFilter =
		vtkSmartPointer<LumenExtractionFilter>::New();
	lumenExtractionFilter->CoreFilter->SetNumberOfIterations(m_numberOfIteractions);
	lumenExtractionFilter->CoreFilter->SetMultiplier(m_multiplier);
	lumenExtractionFilter->CoreFilter->SetInitialNeighborhoodRadius(2);
	for (list<int*>::const_iterator cit = m_seeds.cbegin(); cit != m_seeds.cend(); ++cit) {
		IndexType index = {
			(*cit)[0], (*cit)[1], (*cit)[2] };
		lumenExtractionFilter->CoreFilter->AddSeed(index);
	}
	//lumenExtractionFilter->SetInputData(m_listOfVtkImages[index]);
	lumenExtractionFilter->SetInputConnection(extractVOI->GetOutputPort());
	lumenExtractionFilter->Update();




	for (int i = GetImageViewer()->GetDisplayExtent()[0];
		i <= GetImageViewer()->GetDisplayExtent()[1]; ++i) {
		for (int j = GetImageViewer()->GetDisplayExtent()[2];
			j <= GetImageViewer()->GetDisplayExtent()[3]; ++j) {
			for (int k = GetImageViewer()->GetDisplayExtent()[4];
				k <= GetImageViewer()->GetDisplayExtent()[5]; ++k) {
				unsigned char* pixelLayer = static_cast<unsigned char*>
					(GetImageViewer()->GetInputLayer()->GetScalarPointer(i, j, k));
				unsigned char* pixel = static_cast<unsigned char*>
					(lumenExtractionFilter->GetOutput()->GetScalarPointer(i, j, k));
				*pixelLayer = *pixel;
			}
		}
	}

	GetImageViewer()->GetInputLayer()->Modified();
	//MY_VIEWER_CONSTITERATOR(Render());
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, GetImageViewer()->Render());

	//ExtractLumenPolyData();
}

void QInteractorStyleLumenSeedsPlacer::ExtractSegmentation(std::list<int*>& seed)
{
	vtkSmartPointer<vtkPolyData> splinePoints =
		vtkSmartPointer<vtkPolyData>::New();
	splinePoints->SetPoints(vtkSmartPointer<vtkPoints>::New());
	for (list<int*>::const_iterator cit = seed.cbegin(); cit != seed.cend(); ++cit) {
		splinePoints->GetPoints()->InsertNextPoint((*cit)[0], (*cit)[1], (*cit)[2]);
		//double worldPos[3];
		//for (int pos = 0; pos < 3; ++pos) {
		//	worldPos[pos] = ((*cit)[pos] * GetSpacing()[pos]) + GetOrigin()[pos];
		//}
		//splinePoints->GetPoints()->InsertNextPoint(worldPos);
	}
	
	vtkSmartPointer<vtkTransform> translation =
		vtkSmartPointer<vtkTransform>::New();
	// because default is premultiply 
	// T* (S * Points)
	translation->Translate(GetOrigin());
	translation->Scale(GetSpacing());
	translation->Update();

	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter->SetInputData(splinePoints);
	transformFilter->SetTransform(translation);
	transformFilter->Update();


	vtkSmartPointer<vtkParametricSpline> spline =
		vtkSmartPointer<vtkParametricSpline>::New();
	//spline->SetPoints(splinePoints->GetPoints());
	spline->SetPoints(transformFilter->GetOutput()->GetPoints());

	vtkSmartPointer<vtkParametricFunctionSource> functionSource =
		vtkSmartPointer<vtkParametricFunctionSource>::New();
	functionSource->SetParametricFunction(spline);
	functionSource->Update();

	vtkSmartPointer<vtkPolylineToTubularVolume> polylineToTubularVolume =
		vtkSmartPointer<vtkPolylineToTubularVolume>::New();
	polylineToTubularVolume->SetInputData(GetImageViewer()->GetInputLayer());
	polylineToTubularVolume->SetPolyline(functionSource->GetOutput());
	polylineToTubularVolume->SetTubeRadius(m_extractRadius);
	polylineToTubularVolume->Update();

	vtkSmartPointer<vtkImageMask> maskFilter = vtkSmartPointer<vtkImageMask>::New();
	maskFilter->SetInput1Data(GetImageViewer()->GetInputLayer());
	maskFilter->SetMaskInputData(polylineToTubularVolume->GetOutput());
	maskFilter->Update();

	GetImageViewer()->GetInputLayer()->ShallowCopy(maskFilter->GetOutput());
	GetImageViewer()->GetInputLayer()->Modified();
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, GetImageViewer()->Render());

}

void QInteractorStyleLumenSeedsPlacer::ExtractSegmentation()
{
	ExtractSegmentation(m_seeds);
}

void QInteractorStyleLumenSeedsPlacer::SetExtractRadius(int radius)
{
	this->m_extractRadius = radius;
}

void QInteractorStyleLumenSeedsPlacer::SetMultipier(double value)
{
	m_multiplier = value;
}

void QInteractorStyleLumenSeedsPlacer::SetNumberOfIteractions(int value)
{
	m_numberOfIteractions = value;
}

void QInteractorStyleLumenSeedsPlacer::SetInitialNeighborhoodRadius(int value)
{
	m_initialNeighborhoodRadius = value;
}

QInteractorStyleLumenSeedsPlacer::QInteractorStyleLumenSeedsPlacer(int uiType, QWidget* parent)
{
	QNEW_UI();
}

QInteractorStyleLumenSeedsPlacer::~QInteractorStyleLumenSeedsPlacer()
{
	QDELETE_UI();

}
//
//void QInteractorStyleLumenSeedsPlacer::uniqueInvoke(bool flag)
//{
//	QAbstractNavigation::uniqueInvoke(flag);
//
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
//	}
//	if (flag != initializationFlag) {
//
//	}
//	initializationFlag = flag;
//}

void QInteractorStyleLumenSeedsPlacer::OnKeyPress()
{
	string key = this->Interactor->GetKeySym();
	cout << __func__ << ' ' << key << endl;
	if (key == "D") {
		InteractorStyleSeedsPlacer::OnKeyPress();
	}
	else {
		InteractorStyleSeedsPlacer::OnKeyPress();
	}
}

void QInteractorStyleLumenSeedsPlacer::uniqueInitialization()
{
	connect(ui->pushBtnDeleteSeed, SIGNAL(clicked()),
		this, SLOT(DeleteFocalSeed()));

	connect(ui->listWidgetSeedList, SIGNAL(currentRowChanged(int)),
		this, SLOT(SetFocalSeed(int)));

	connect(ui->dropSeedPushButton, SIGNAL(clicked()),
		this, SLOT(DropSeed()));
	// only create lumenExtraction and connect its parameter in the first 
	// InteractorStyleLumenSeedsPalcer, others' are nullptr and won't be 
	// connected
	connect(ui->pushBtnExtractLumen, SIGNAL(clicked()),
		this, SLOT(ExtractLumen()));
	connect(ui->doubleSpinBoxMultiplier, SIGNAL(valueChanged(double)),
		this, SLOT(SetMultipier(double)));
	connect(ui->numberOfIterationsSpinBox, SIGNAL(valueChanged(int)),
		this, SLOT(SetNumberOfIteractions(int)));
	connect(ui->initialNeighbodhoodSpinBox, SIGNAL(valueChanged(int)),
		this, SLOT(SetInitialNeighborhoodRadius(int)));
	connect(ui->spinBoxExtractRadius, SIGNAL(valueChanged(int)),
		this, SLOT(SetExtractRadius(int)));
	connect(ui->pushBtnExtractSegmentation, SIGNAL(clicked()),
		this, SLOT(ExtractSegmentation()));
}

void QInteractorStyleLumenSeedsPlacer::initialization()
{
	connect(ui->deleteAllSeedsPushButton, SIGNAL(clicked()),
		this, SLOT(SlotClearAllSeeds()));
}
