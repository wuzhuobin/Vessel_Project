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
#include <vtkHandleWidget.h>
#include <vtkHandleRepresentation.h>
#include <vtkSeedRepresentation.h>
//#include <vtkCommand.h>
#include <vtkRenderWindow.h>

#include "vtkPolylineToTubularVolume.h"
#include "LumenExtractionFilter.h"
#include "ImageViewer.h"
#include "ui_QInteractorStyleLumenSeedsPlacer.h"
#include "ui_QAbstractNavigation.h"

#include <qevent.h>

vtkStandardNewMacro(QInteractorStyleLumenSeedsPlacer);
QSETUP_UI_SRC(QInteractorStyleLumenSeedsPlacer);
QList<int*> QInteractorStyleLumenSeedsPlacer::m_lumenSeeds;

using namespace std;
//class UpdateTargetViewerCallback: public vtkCommand
//{
//public:
//	static UpdateTargetViewerCallback* New() { return new UpdateTargetViewerCallback; }
//
//	virtual void Execute(vtkObject *caller, unsigned long eventId,
//		void *callData) {
//		self->UpdateTargetViewer();
//	}
//	QInteractorStyleLumenSeedsPlacer* self = nullptr;
//};

void QInteractorStyleLumenSeedsPlacer::SetCustomEnabled(bool flag)
{
	InteractorStyleSeedsPlacer::SetCustomEnabled(flag);
	uniqueInvoke(flag);


	//if (flag) {
	//	callback = UpdateTargetViewerCallback::New();
	//	callback->self = this;
	//	GetImageViewer()->GetRenderWindow()->AddObserver(vtkCommand::RenderEvent, callback);
	//}
	//else
	//{
	//	GetImageViewer()->GetRenderWindow()->RemoveObserver(callback);
	//	callback->Delete();
	//}


}

void QInteractorStyleLumenSeedsPlacer::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleSeedsPlacer::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStyleLumenSeedsPlacer::UpdateTargetViewer()
{
	ui->comboBoxTargeImage->clear();
	m_listOfModalityNames.clear();
	for (list<AbstractInteractorStyle*>::const_iterator cit = m_abstractInteractorStyles.cbegin();
		cit != m_abstractInteractorStyles.cend(); ++cit) {
		QInteractorStyleLumenSeedsPlacer* _style = dynamic_cast<QInteractorStyleLumenSeedsPlacer*>(*cit);
		if (_style && _style->GetCustomEnabled()) {
			m_listOfModalityNames.append(QString::fromStdString(_style->GetImageViewer()->GetWindowName()));
		}
	}
	m_listOfModalityNames.removeDuplicates();
	for (int i = 0; i < m_listOfModalityNames.size(); ++i) {
		ui->comboBoxTargeImage->addItem(m_listOfModalityNames[i]);
	}
}

void QInteractorStyleLumenSeedsPlacer::UpdateWidgetToSeeds(int * newImagePos, int* oldImagePos)
{
	QInteractorStyleLumenSeedsPlacer::UpdateWidgetToSeeds(m_lumenSeeds, newImagePos, oldImagePos);
}

void QInteractorStyleLumenSeedsPlacer::UpdateWidgetToSeeds(QList<int*>& seeds, int * newImagePos, int * oldImagePos)
{
	// if oldImagePos is nullptr or oldImagePos is not a valid number, 
	// replace the existed newImagePos or push the new newImagePos
	if (oldImagePos == nullptr ||
		oldImagePos[0] < 0 ||
		oldImagePos[1] < 0 ||
		oldImagePos[2] < 0) {
		oldImagePos = newImagePos;
	}
	QList<int*>::iterator it = std::find_if(seeds.begin(),
		seeds.end(), [oldImagePos](int* index)->bool {
		return
			oldImagePos[0] == index[0] &&
			oldImagePos[1] == index[1] &&
			oldImagePos[2] == index[2];
	});
	if (it != seeds.end()) {
		memcpy(*it, newImagePos, sizeof(int[3]));
		QString listItem = "Seed Index: [" +
			QString::number(newImagePos[0]) + "," +
			QString::number(newImagePos[1]) + "," +
			QString::number(newImagePos[2]) + "]";
		ui->listWidgetSeedList->item(m_lumenSeeds.indexOf(*it))->setText(listItem);
	}
	else {
		seeds.push_back(new int[3]);
		memcpy(seeds.back(), newImagePos, sizeof(int[3]));
		QString listItem = "Seed Index: [" +
			QString::number(seeds.back()[0]) + "," +
			QString::number(seeds.back()[1]) + "," +
			QString::number(seeds.back()[2]) + "]";
		ui->listWidgetSeedList->addItem(listItem);
	}
}

void QInteractorStyleLumenSeedsPlacer::ClearAllSeeds(QList<int*>& seed)
{
	if (seed.size() != 0) {
		while (!seed.empty())
		{
			delete[] seed.back();
			seed.pop_back();
		}
	}
}

void QInteractorStyleLumenSeedsPlacer::DeleteFocalSeed(QList<int*>& seeds)
{
	int i = ui->listWidgetSeedList->currentRow();
	if (i >= 0 && i < seeds.size()) {
		ui->listWidgetSeedList->removeItemWidget(
			ui->listWidgetSeedList->takeItem(i));
		delete[] seeds[i];
		seeds.removeAt(i);
	}
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, ClearAllSeedWidget());
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, GenerateWidgetFromSeeds());
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, m_seedWidget->Render());
}

void QInteractorStyleLumenSeedsPlacer::ExtractLumen(QList<int*>& seeds)
{
	typedef itk::Index<3> IndexType;
	ImageViewer* inputViewer;
	for (list<AbstractInteractorStyle*>::const_iterator cit = m_abstractInteractorStyles.cbegin();
		cit != m_abstractInteractorStyles.cend(); ++cit) {
		QInteractorStyleLumenSeedsPlacer* _style = dynamic_cast<QInteractorStyleLumenSeedsPlacer*>(*cit);
		
		if (_style && _style->GetCustomEnabled() && 
			QString::fromStdString(_style->GetImageViewer()->GetWindowName()) == 
			ui->comboBoxTargeImage->currentText()) {
			inputViewer = GetImageViewer();
		}
	}


	vtkSmartPointer<vtkExtractVOI> extractVOI =
		vtkSmartPointer<vtkExtractVOI>::New();
	extractVOI->SetInputData(inputViewer->GetInput());
	extractVOI->SetVOI(inputViewer->GetDisplayExtent());
	extractVOI->Update();

	vtkSmartPointer<LumenExtractionFilter> lumenExtractionFilter =
		vtkSmartPointer<LumenExtractionFilter>::New();
	lumenExtractionFilter->CoreFilter->SetNumberOfIterations(m_numberOfIteractions);
	lumenExtractionFilter->CoreFilter->SetMultiplier(m_multiplier);
	lumenExtractionFilter->CoreFilter->SetInitialNeighborhoodRadius(2);
	for (QList<int*>::const_iterator cit = seeds.cbegin(); cit != seeds.cend(); ++cit) {
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

void QInteractorStyleLumenSeedsPlacer::ClearAllSeeds()
{
	ClearAllSeeds(m_lumenSeeds);
	ui->listWidgetSeedList->clear();
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleLumenSeedsPlacer, ClearAllSeedWidget());
	//m_seedWidget->Render();
}

void QInteractorStyleLumenSeedsPlacer::SetFocalSeed(int i)
{
	QInteractorStyleLumenSeedsPlacer::SetFocalSeed(i, m_lumenSeeds);

	const double* worldCoordinate = GetImageViewer()->GetFocalPointWithWorldCoordinate();

	//char label[60];
	//sprintf_s(label, "(%.3f, %.3f, %.3f)", worldCoordinate[0], worldCoordinate[1], worldCoordinate[2]);
	QString label = "(" + QString::number(worldCoordinate[0], 'f', 3) + "," +
		QString::number(worldCoordinate[1], 'f', 3) + "," +
		QString::number(worldCoordinate[2], 'f', 3) + ")";
	/// change label text
	this->ui->labelSeedCoordinate->setText(label);
}

void QInteractorStyleLumenSeedsPlacer::SetFocalSeed(int i, QList<int*>& seeds)
{
	if (i < 0 || i > seeds.size()) {
		return;
	}
	int imageCoordinate[3];
	memcpy(imageCoordinate, seeds[i], sizeof(imageCoordinate));
	SetCurrentFocalPointWithImageCoordinate(imageCoordinate[0], imageCoordinate[1],
		imageCoordinate[2]);
}

void QInteractorStyleLumenSeedsPlacer::GenerateWidgetFromSeeds()
{
	GenerateWidgetFromSeeds(m_lumenSeeds);
}

void QInteractorStyleLumenSeedsPlacer::GenerateWidgetFromSeeds(const QList<int*>& seeds)
{
	ClearAllSeedWidget();

	for (QList<int*>::const_iterator cit = seeds.cbegin();
		cit != seeds.cend(); ++cit) {
		int* imagePos = (*cit);
		double worldPos[3];
		for (int pos = 0; pos < 3; ++pos) {
			worldPos[pos] = (imagePos[pos] * GetSpacing()[pos]) + GetOrigin()[pos];
		}
		if (imagePos[GetSliceOrientation()] == GetImageViewer()->GetSlice()) {
			vtkHandleWidget* newSeed = m_seedWidget->CreateNewHandle();
			newSeed->GetHandleRepresentation()->SetWorldPosition(worldPos);
			newSeed->EnabledOn();
		}
	}
	m_seedWidget->Render();
}

void QInteractorStyleLumenSeedsPlacer::SaveWidgetToSeeds()
{
	SaveWidgetToSeeds(m_lumenSeeds);
}

void QInteractorStyleLumenSeedsPlacer::DeleteFocalSeed()
{
	DeleteFocalSeed(m_lumenSeeds);
}

void QInteractorStyleLumenSeedsPlacer::SaveWidgetToSeeds(QList<int*>& seeds)
{
	for (int i = m_seedRep->GetNumberOfSeeds() - 1; i >= 0; --i) {
		//double* worldPos = new double[3]; // #MemLeakHere
		double worldPos[3];
		m_seedWidget->GetSeedRepresentation()->GetSeedWorldPosition(i, worldPos);

		int imagePos[3];
		for (int pos = 0; pos < 3; ++pos) {
			imagePos[pos] = (worldPos[pos] - GetOrigin()[pos]) / GetSpacing()[pos] + 0.5;
		}
		UpdateWidgetToSeeds(seeds, imagePos);
	}
}

void QInteractorStyleLumenSeedsPlacer::DropSeed(QList<int*>& seeds)
{
	double* worldPos = GetImageViewer()->GetFocalPointWithWorldCoordinate();
	vtkHandleWidget* newSeed = m_seedWidget->CreateNewHandle();
	newSeed->GetHandleRepresentation()->SetWorldPosition(worldPos);
	newSeed->EnabledOn();
	m_seedWidget->Render();

	SaveWidgetToSeeds(seeds);
	OnLeftButtonUp();
}

void QInteractorStyleLumenSeedsPlacer::DropSeed()
{
	DropSeed(m_lumenSeeds);
}

void QInteractorStyleLumenSeedsPlacer::ExtractLumen()
{
	ExtractLumen(m_lumenSeeds);
}

void QInteractorStyleLumenSeedsPlacer::ExtractSegmentation(QList<int*>& seed)
{
	vtkSmartPointer<vtkPolyData> splinePoints =
		vtkSmartPointer<vtkPolyData>::New();
	splinePoints->SetPoints(vtkSmartPointer<vtkPoints>::New());
	for (QList<int*>::const_iterator cit = seed.cbegin(); cit != seed.cend(); ++cit) {
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
	ExtractSegmentation(m_lumenSeeds);
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
	std::string key = this->Interactor->GetKeySym();
	cout << __func__ << ' ' << key << endl;
	if (key == "D") {
		InteractorStyleSeedsPlacer::OnKeyPress();
	}
	else {
		InteractorStyleSeedsPlacer::OnKeyPress();
	}
}

void QInteractorStyleLumenSeedsPlacer::enterEvent(QEvent * event)
{
	QWidget::enterEvent(event);
	UpdateTargetViewer();
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
		this, SLOT(ClearAllSeeds()));
}

void QInteractorStyleLumenSeedsPlacer::uniqueEnable()
{
	QAbstractNavigation::uniqueEnable();
	UpdateTargetViewer();
}
