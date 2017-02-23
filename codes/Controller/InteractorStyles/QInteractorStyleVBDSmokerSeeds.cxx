#include <vtkObjectFactory.h>
#include "QInteractorStyleVBDSmokerSeeds.h"
#include "ui_QInteractorStyleVBDSmokerSeeds.h"
#include "ImageViewer.h"

#include <vtkHandleWidget.h>


vtkStandardNewMacro(QInteractorStyleVBDSmokerSeeds);
QSETUP_UI_SRC(QInteractorStyleVBDSmokerSeeds);
std::list<int*> QInteractorStyleVBDSmokerSeeds::m_VBDSmokerSeeds;

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

void QInteractorStyleVBDSmokerSeeds::GenerateWidgetFromSeeds()
{
	InteractorStyleSeedsPlacer::GenerateWidgetFromSeeds(m_VBDSmokerSeeds);
}

void QInteractorStyleVBDSmokerSeeds::SaveWidgetToSeeds()
{
	InteractorStyleSeedsPlacer::SaveWidgetToSeeds(m_VBDSmokerSeeds);

}

void QInteractorStyleVBDSmokerSeeds::DropSeed()
{
	InteractorStyleSeedsPlacer::DropSeed(m_VBDSmokerSeeds);

}

void QInteractorStyleVBDSmokerSeeds::UpdateWidgetToSeeds(int * newImagePos, int * oldImagePos)
{
	InteractorStyleSeedsPlacer::UpdateWidgetToSeeds(m_VBDSmokerSeeds, newImagePos, oldImagePos);
}

void QInteractorStyleVBDSmokerSeeds::UpdateWidgetToSeeds(std::list<int*>& seeds, int * newImagePos, int * oldImagePos)
{
	InteractorStyleSeedsPlacer::UpdateWidgetToSeeds(seeds, newImagePos, oldImagePos);
}

void QInteractorStyleVBDSmokerSeeds::ClearAllSeeds()
{
	InteractorStyleSeedsPlacer::ClearAllSeeds(m_VBDSmokerSeeds);
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

	connect(ui->pushButtonBasilarArteryBifurcationLocation, SIGNAL(clicked()),
		this, SLOT(slotBasilarArteryBifurcationLocationCurrentSlice()));

	connect(ui->pushButtonPonsCentralSectionLocation, SIGNAL(clicked()),
		this, SLOT(slotPonsCentralSectionLocationCurrentSlice()));
}

void QInteractorStyleVBDSmokerSeeds::initialization()
{
	QAbstractNavigation::initialization();
}
