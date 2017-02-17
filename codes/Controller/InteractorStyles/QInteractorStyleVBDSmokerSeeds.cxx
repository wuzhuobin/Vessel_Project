#include <vtkObjectFactory.h>
#include "QInteractorStyleVBDSmokerSeeds.h"
#include "ImageViewer.h"


#include "ui_QInteractorStyleVBDSmokerSeeds.h"

#include <qmessagebox.h>
#include <qlist.h>

using namespace std;

vtkStandardNewMacro(QInteractorStyleVBDSmokerSeeds);
QSETUP_UI_SRC(QInteractorStyleVBDSmokerSeeds);

void QInteractorStyleVBDSmokerSeeds::SetCustomEnabled(bool flag)
{
	InteractorStyleSeedsPlacer::SetCustomEnabled(flag);
	uniqueInvoke(flag);
}

void QInteractorStyleVBDSmokerSeeds::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleSeedsPlacer::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}


void QInteractorStyleVBDSmokerSeeds::SlotClearAllSeeds()
{
	ClearAllSeeds();
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleVBDSmokerSeeds, ClearAllSeedWidget());
	ui->listWidgetSeedList->clear();
	m_imageViewer->Render();
}

void QInteractorStyleVBDSmokerSeeds::SetFocalSeed(int i)
{
	if (i < 0)
		return;
	QList<int*> _seeds = QList<int*>::fromStdList(m_seeds);
	int imageCoordinate[3];
	double worldCoordinate[3];

	memcpy(imageCoordinate, _seeds[i], sizeof(imageCoordinate));
	SetCurrentFocalPointWithImageCoordinate(imageCoordinate[0], imageCoordinate[1],
		imageCoordinate[2]);

	for (int pos = 0; pos < 3; ++pos) {
		worldCoordinate[pos] = imageCoordinate[pos] * GetSpacing()[pos] - GetOrigin()[i];
	}

	//char label[60];
	//sprintf_s(label, "(%.3f, %.3f, %.3f)", worldCoordinate[0], worldCoordinate[1], worldCoordinate[2]);
	QString label = "(" + QString::number(worldCoordinate[0], 'f', 3) + "," +
		QString::number(worldCoordinate[1], 'f', 3) + "," +
		QString::number(worldCoordinate[2], 'f', 3) + ")";
	/// change label text
	this->ui->labelSeedCoordinate->setText(label);
}

void QInteractorStyleVBDSmokerSeeds::DeleteFocalSeed()
{
	QList<int*> _seeds = QList<int*>::fromStdList(m_seeds);
	int i = ui->listWidgetSeedList->currentRow();
	if (i >= 0 && i < _seeds.size()) {
		ui->listWidgetSeedList->removeItemWidget(
			ui->listWidgetSeedList->takeItem(i));
		m_seeds.remove(_seeds[i]);
		delete _seeds[i];
	}
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleVBDSmokerSeeds, ClearAllSeedWidget());
	STYLE_DOWN_CAST_CONSTITERATOR(QInteractorStyleVBDSmokerSeeds, GenerateWidgetFromSeeds());
	MY_VIEWER_CONSTITERATOR(Render());
}

void QInteractorStyleVBDSmokerSeeds::SaveWidgetToSeeds()
{
	InteractorStyleSeedsPlacer::SaveWidgetToSeeds();
	ui->listWidgetSeedList->clear();
	for (list<int*>::const_iterator cit = m_seeds.cbegin(); cit != m_seeds.cend(); ++cit) {
		QString listItem = "Seed Index: [" +
			QString::number((*cit)[0]) + "," +
			QString::number((*cit)[1]) + "," +
			QString::number((*cit)[2]) + "]";
		ui->listWidgetSeedList->addItem(listItem);
		//++i;
	}
}

void QInteractorStyleVBDSmokerSeeds::DropSeed()
{
	InteractorStyleSeedsPlacer::DropSeed();
}
#include <vtkExtractVOI.h>
#include <vtkSmartPointer.h>
#include <vtkImageAccumulate.h>
#include <vtkImageData.h>
void QInteractorStyleVBDSmokerSeeds::SlotVBDSmokerUpdate()
{
	if (m_seeds.size() < 11) {
		QMessageBox::critical(this, "Not enough points", "At least 11 points");
		return;
	}
	QList<int*> _seeds = QList<int*>::fromStdList(m_seeds);

	for (int i = 0; i < 4; ++i) {
		vtkSmartPointer<vtkExtractVOI> extractVOI =
			vtkSmartPointer<vtkExtractVOI>::New();
		extractVOI->SetInputData(m_imageViewer->GetInputLayer());
		extractVOI->SetVOI(m_imageViewer->GetDisplayExtent()[0], m_imageViewer->GetDisplayExtent()[1], m_imageViewer->GetDisplayExtent()[2], m_imageViewer->GetDisplayExtent()[3],
			_seeds[i][2], _seeds[i + 1][2]);
		if (i == 3) {
			extractVOI->SetVOI(m_imageViewer->GetDisplayExtent()[0], m_imageViewer->GetDisplayExtent()[1], m_imageViewer->GetDisplayExtent()[2], m_imageViewer->GetDisplayExtent()[3],
				_seeds[i][2], m_imageViewer->GetDisplayExtent()[5]);
		}
		extractVOI->Update();

		vtkSmartPointer<vtkImageAccumulate> imageAccumulate =
			vtkSmartPointer<vtkImageAccumulate>::New();
		imageAccumulate->SetInputConnection(extractVOI->GetOutputPort());
		imageAccumulate->SetComponentExtent(0, 0, 0, 0, 0, 0);
		imageAccumulate->SetComponentOrigin(1, 0, 0);
		imageAccumulate->SetComponentSpacing(1, 0, 0);
		imageAccumulate->IgnoreZeroOn();
		imageAccumulate->Update();
		int* output = static_cast<int*>(imageAccumulate->GetOutput()->GetScalarPointer());
		cout <<i << " " << output[0] << endl;
		if (output[0] > 1) {
			ui->spinBoxProlongation->setValue(i);
		}
	}

}


QInteractorStyleVBDSmokerSeeds::QInteractorStyleVBDSmokerSeeds(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleVBDSmokerSeeds::~QInteractorStyleVBDSmokerSeeds()
{
	QDELETE_UI();
}

void QInteractorStyleVBDSmokerSeeds::UpdateWidgetToSeeds(int * oldImagePos, int * newImagePos)
{
	InteractorStyleSeedsPlacer::UpdateWidgetToSeeds(oldImagePos, newImagePos);
	ui->listWidgetSeedList->clear();
	//QList<int*> _seeds = QList<int*>::fromStdList(m_seeds);
	//for (QList<int*>::ConstIterator cit = _seeds.cbegin(); cit != _seeds.cend(); ++cit) {
	//	ui->listWidgetSeedList->insertItem()
	//}
	//int i = 0;
	for (list<int*>::const_iterator cit = m_seeds.cbegin(); cit != m_seeds.cend(); ++cit) {
		QString listItem = "Seed Index: [" +
			QString::number((*cit)[0]) + "," +
			QString::number((*cit)[1]) + "," +
			QString::number((*cit)[2]) + "]";
		ui->listWidgetSeedList->addItem(listItem);
		//++i;
	}
}

void QInteractorStyleVBDSmokerSeeds::uniqueInitialization()
{
	connect(ui->pushButtonDeleteSeed, SIGNAL(clicked()),
		this, SLOT(DeleteFocalSeed()));

	connect(ui->listWidgetSeedList, SIGNAL(currentRowChanged(int)),
		this, SLOT(SetFocalSeed(int)));

	connect(ui->pushButtonDropSeed, SIGNAL(clicked()),
		this, SLOT(DropSeed()));

	connect(ui->pushButtonUpdate, SIGNAL(clicked()),
		this, SLOT(SlotVBDSmokerUpdate()));
}

void QInteractorStyleVBDSmokerSeeds::initialization()
{
	connect(ui->pushButtonDeleteAllSeeds, SIGNAL(clicked()),
		this, SLOT(SlotClearAllSeeds()));
}
