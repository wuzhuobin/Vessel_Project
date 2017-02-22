#include <vtkObjectFactory.h>
#include "QInteractorStyleVBDSmokerSeeds.h"
#include "ui_QInteractorStyleVBDSmokerSeeds.h"
#include "ImageViewer.h"

#include <vtkHandleWidget.h>


vtkStandardNewMacro(QInteractorStyleVBDSmokerSeeds);
QSETUP_UI_SRC(QInteractorStyleVBDSmokerSeeds);

void QInteractorStyleVBDSmokerSeeds::SetCustomEnabled(bool flag)
{
	InteractorStyleNavigation::SetCustomEnabled(flag);
	uniqueInvoke(flag);
}

void QInteractorStyleVBDSmokerSeeds::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStyleVBDSmokerSeeds::uniqueEnable()
{
	QAbstractNavigation::uniqueEnable();
	int* extent = GetExtent();
	ui->spinBoxBasilarArteryBifurcationLocation->setRange(extent[4], extent[5]);
	ui->spinBoxPonsCentralSectionLocation->setRange(extent[4], extent[5]);

	for (int i = 0; i < NUM_OF_SPHERE_WIDGET; i++)
	{
		m_handleWidgets[i] = vtkSmartPointer<vtkHandleWidget>::New();
		(m_handleWidgets[i])->SetInteractor(this->Interactor);
		(m_handleWidgets[i])->CreateDefaultRepresentation();
		
	}

	connect(ui->pushButtonBasilarArteryBifurcationLocation0, SIGNAL(toggled(bool)),
		this, SLOT(slotEnableTest(bool)));
}

void QInteractorStyleVBDSmokerSeeds::uniqueDisable()
{
	for (int i = 0; i < NUM_OF_SPHERE_WIDGET; i++)
	{
		m_handleWidgets[i] = nullptr;
	}
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
	(m_handleWidgets[0])->SetEnabled(flag);
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
