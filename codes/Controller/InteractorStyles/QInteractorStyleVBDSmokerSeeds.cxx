#include "QInteractorStyleVBDSmokerSeeds.h"
#include "ui_QInteractorStyleVBDSmokerSeeds.h"
#include "ImageViewer.h"

#include <vtkObjectFactory.h>
#include <vtkHandleWidget.h>
#include <vtkSeedWidget.h>
#include <vtkSeedRepresentation.h>
#include <vtkPointHandleRepresentation3D.h>

//#include <qpushbutton.h>

using namespace std;
vtkStandardNewMacro(QInteractorStyleVBDSmokerSeeds);
QSETUP_UI_SRC(QInteractorStyleVBDSmokerSeeds);


// using a initial value
list<int*> QInteractorStyleVBDSmokerSeeds::m_VBDSmokerElongation(QInteractorStyleVBDSmokerSeeds::NUM_OF_ELONGATION, new int[3]{ -1,-1,-1 });
list<int*> QInteractorStyleVBDSmokerSeeds::m_VBDSmokerDetour(QInteractorStyleVBDSmokerSeeds::NUM_OF_DETOUR, new int[3]{ -1,-1,-1 });

void QInteractorStyleVBDSmokerSeeds::SetCustomEnabled(bool flag)
{
	InteractorStyleSeedsPlacer::SetCustomEnabled(flag);
	uniqueInvoke(flag);

	if (flag) {
		m_seedWidget->CompleteInteraction();
	}
}

void QInteractorStyleVBDSmokerSeeds::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleSeedsPlacer::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStyleVBDSmokerSeeds::GenerateWidgetFromSeeds()
{
	ClearAllSeedWidget();
	for (list<int*>::const_iterator cit = m_VBDSmokerElongation.cbegin();
		cit != m_VBDSmokerElongation.cend(); ++cit) {
		int* imagePos = (*cit);
		double worldPos[3];
		m_imageViewer->GetFocalPointWithWorldCoordinate(worldPos);
		worldPos[2] = (imagePos[2] * GetSpacing()[2]) + GetOrigin()[2];
		vtkHandleWidget* newSeed = m_seedWidget->CreateNewHandle();
		newSeed->GetHandleRepresentation()->SetWorldPosition(worldPos);

		if (imagePos[0] >= 0) {
			newSeed->EnabledOn();
		}
	}

	for (list<int*>::const_iterator cit = m_VBDSmokerDetour.cbegin();
		cit != m_VBDSmokerDetour.cend(); ++cit) {
		int* imagePos = (*cit);
		double worldPos[3];
		m_imageViewer->GetFocalPointWithWorldCoordinate(worldPos);
		worldPos[0] = (imagePos[0] * GetSpacing()[0]) + GetOrigin()[0];
		worldPos[1] = (imagePos[1] * GetSpacing()[1]) + GetOrigin()[1];
		vtkHandleWidget* newSeed = m_seedWidget->CreateNewHandle();
		newSeed->GetHandleRepresentation()->SetWorldPosition(worldPos);
		
		if (imagePos[0] >= 0 ) {
			newSeed->EnabledOn();
		}
	}
}

//void QInteractorStyleVBDSmokerSeeds::GenerateWidgetFromSeeds(const std::list<int*>& seeds)
//{
//
//}

void QInteractorStyleVBDSmokerSeeds::SaveWidgetToSeeds()
{
	list<int*>::const_iterator cit1 = m_VBDSmokerElongation.cbegin();
	list<int*>::const_iterator cit2 = m_VBDSmokerDetour.cbegin();

	for (int i = 0; i < m_seedRep->GetNumberOfSeeds(); ++i) {
		double worldPos[3];
		m_seedWidget->GetSeedRepresentation()->GetSeedWorldPosition(i, worldPos);

		int imagePos[3];
		for (int pos = 0; pos < 3; ++pos) {
			imagePos[pos] = (worldPos[pos] - GetOrigin()[pos]) / GetSpacing()[pos] + 0.5;
		}
		if (i < NUM_OF_ELONGATION) {
			memcpy(*cit1, imagePos, sizeof(imagePos));
			++cit1;
		}
		else {
			memcpy(*cit2, imagePos, sizeof(imagePos));
			++cit2;
		}
	}
}

void QInteractorStyleVBDSmokerSeeds::DropSeed()
{
	double* worldPos = m_imageViewer->GetFocalPointWithWorldCoordinate();
	list<int*>::const_iterator cit1 = m_VBDSmokerElongation.cbegin();
	list<int*>::const_iterator cit2 = m_VBDSmokerDetour.cbegin();
	QPushButton* _pushButtons[] = {
		ui->pushButtonBasilarArteryBifurcationLocation0,
		ui->pushButtonBasilarArteryBifurcationLocation1,
		ui->pushButtonBasilarArteryBifurcationLocation2,
		ui->pushButtonBasilarArteryBifurcationLocation3,
		ui->pushButtonPonsCentralSectionLocation0,
		ui->pushButtonPonsCentralSectionLocation1Left,
		ui->pushButtonPonsCentralSectionLocation1Right,
		ui->pushButtonPonsCentralSectionLocation2Left,
		ui->pushButtonPonsCentralSectionLocation2Right,
		ui->pushButtonPonsCentralSectionLocation3Left,
		ui->pushButtonPonsCentralSectionLocation3Right
	};
	for (int i = 0; i < NUM_OF_DETOUR + NUM_OF_ELONGATION; ++i) {
		int* imagePos;
		if (i < NUM_OF_ELONGATION) {
			imagePos = (*cit1);
			cit1++;
		}
		else {
			imagePos = (*cit2);
			cit2++;
		}

		if (sender() == _pushButtons[i]) {
			double worldPos[3];
			int* imagePos = (*cit1);
			if (imagePos[0] >= 0) {
				for (int pos = 0; pos < 3; ++pos) {
					worldPos[pos] = (imagePos[pos] * GetSpacing()[pos]) + GetOrigin()[pos];
				}
			}
			else {
				m_imageViewer->GetFocalPointWithWorldCoordinate(worldPos);
			}
			m_seedWidget->GetSeed(i)->GetHandleRepresentation()->SetWorldPosition(worldPos);
			m_seedWidget->GetSeed(i)->SetEnabled(_pushButtons[i]->isChecked());
			break;
		}
	}

	InteractorStyleSeedsPlacer::DropSeed(m_VBDSmokerElongation);

}

void QInteractorStyleVBDSmokerSeeds::UpdateWidgetToSeeds(int * newImagePos, int * oldImagePos)
{
	// do nothing
}

void QInteractorStyleVBDSmokerSeeds::UpdateWidgetToSeeds(std::list<int*>& seeds, int * newImagePos, int * oldImagePos)
{
	// do nothing
}

void QInteractorStyleVBDSmokerSeeds::ClearAllSeeds()
{
	InteractorStyleSeedsPlacer::ClearAllSeeds(m_VBDSmokerElongation);
	InteractorStyleSeedsPlacer::ClearAllSeeds(m_VBDSmokerDetour);
}

void QInteractorStyleVBDSmokerSeeds::uniqueEnable()
{
	QAbstractNavigation::uniqueEnable();
	int* extent = GetExtent();
	ui->spinBoxBasilarArteryBifurcationLocation->setRange(extent[4], extent[5]);
	ui->spinBoxPonsCentralSectionLocation->setRange(extent[4], extent[5]);



	connect(ui->pushButtonBasilarArteryBifurcationLocation0, SIGNAL(toggled(bool)),
		this, SLOT(slotEnableTest(bool)));
}

void QInteractorStyleVBDSmokerSeeds::uniqueDisable()
{
}

void QInteractorStyleVBDSmokerSeeds::slotBasilarArteryBifurcationLocationCurrentSlice() 
{
	int ijk[3];
	m_imageViewer->GetFocalPointWithImageCoordinate(ijk);
	ui->spinBoxBasilarArteryBifurcationLocation->setValue(ijk[2]);
}

void QInteractorStyleVBDSmokerSeeds::slotPonsCentralSectionLocationCurrentSlice()
{
	int ijk[3];
	m_imageViewer->GetFocalPointWithImageCoordinate(ijk);
	ui->spinBoxPonsCentralSectionLocation->setValue(ijk[2]);
}

void QInteractorStyleVBDSmokerSeeds::slotEnableTest(bool flag)
{
}

QInteractorStyleVBDSmokerSeeds::QInteractorStyleVBDSmokerSeeds(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleVBDSmokerSeeds::~QInteractorStyleVBDSmokerSeeds()
{
	QDELETE_UI();
}


void QInteractorStyleVBDSmokerSeeds::uniqueInitialization()
{
	QAbstractNavigation::uniqueInitialization();

	QPushButton* _pushButtons[] = {
		ui->pushButtonBasilarArteryBifurcationLocation0,
		ui->pushButtonBasilarArteryBifurcationLocation1,
		ui->pushButtonBasilarArteryBifurcationLocation2,
		ui->pushButtonBasilarArteryBifurcationLocation3,
		ui->pushButtonPonsCentralSectionLocation0,
		ui->pushButtonPonsCentralSectionLocation1Left,
		ui->pushButtonPonsCentralSectionLocation1Right,
		ui->pushButtonPonsCentralSectionLocation2Left,
		ui->pushButtonPonsCentralSectionLocation2Right,
		ui->pushButtonPonsCentralSectionLocation3Left,
		ui->pushButtonPonsCentralSectionLocation3Right
	};
	for (int i = 0; i < NUM_OF_DETOUR + NUM_OF_ELONGATION; ++i) {
		connect(_pushButtons[i], SIGNAL(toggled(bool)), this, SLOT(DropSeed()));
	}

	connect(ui->pushButtonBasilarArteryBifurcationLocation, SIGNAL(clicked()),
		this, SLOT(slotBasilarArteryBifurcationLocationCurrentSlice()));

	connect(ui->pushButtonPonsCentralSectionLocation, SIGNAL(clicked()),
		this, SLOT(slotPonsCentralSectionLocationCurrentSlice()));
}

void QInteractorStyleVBDSmokerSeeds::initialization()
{
	QAbstractNavigation::initialization();
}
