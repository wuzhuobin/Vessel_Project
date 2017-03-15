#include "QInteractorStyleNavigation.h"
#include "ui_qinteractorstylenavigation.h"
#include "ui_QAbstractNavigation.h"
#include "ImageViewer.h"

#include <vtkObjectFactory.h>
#include <vtkCallbackCommand.h>
//#include <vtkImageData.h>

vtkStandardNewMacro(QInteractorStyleNavigation);
QSETUP_UI_SRC(QInteractorStyleNavigation);



void QInteractorStyleNavigation::SetCustomEnabled(bool flag)
{
	InteractorStyleNavigation::SetCustomEnabled(flag);
	uniqueInvoke(flag);
	if (flag) {
		SetExtentRange(GetImageViewer()->GetDisplayExtent());
	}
}

void QInteractorStyleNavigation::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStyleNavigation::SetViewer(vtkObject * viewer)
{
	InteractorStyleNavigation::SetViewer(viewer);
}

QInteractorStyleNavigation::QInteractorStyleNavigation(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleNavigation::~QInteractorStyleNavigation() {
	QDELETE_UI();
}

void QInteractorStyleNavigation::uniqueEnable()
{
	QAbstractNavigation::uniqueEnable();
}

void QInteractorStyleNavigation::initialization()
{
	//callback = vtkCallbackCommand::New();
	//callback->SetClientData(this);
	//callback->SetCallback([](vtkObject* caller, unsigned long, void* clientData, void*) {
	//	ImageViewer* viewer = reinterpret_cast<ImageViewer*>(caller);
	//	QInteractorStyleNavigation* self = reinterpret_cast<QInteractorStyleNavigation*>(clientData);
	//	if (viewer->GetInput()) {
	//		cout << "extent" << endl;
	//		cout << viewer->GetDisplayExtent()[0] << endl;
	//		cout << viewer->GetDisplayExtent()[1] << endl;
	//		cout << viewer->GetDisplayExtent()[2] << endl;
	//		cout << viewer->GetDisplayExtent()[3] << endl;
	//		cout << viewer->GetDisplayExtent()[4] << endl;
	//		cout << viewer->GetDisplayExtent()[5] << endl;
	//		self->SetExtentRange(viewer->GetDisplayExtent());
	//	}

	//});
}

void QInteractorStyleNavigation::destroy()
{
	//callback->Delete();
}
