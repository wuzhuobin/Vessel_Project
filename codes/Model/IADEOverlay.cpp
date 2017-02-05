#include "IADEOverlay.h"

#include <vtkLookupTable.h>

IADEOverlay::IADEOverlay(QObject * parent)
	:Overlay(parent)
{
	// using m_overlayColor to build the lookupTable
	m_overlayColor[0] = zeros;
	m_overlayColor[1] = lumen;
	m_overlayColor[2] = vesselWall;
	m_overlayColor[3] = calcification;
	m_overlayColor[4] = hemorrhage;
	m_overlayColor[5] = lrnc;
	m_overlayColor[6] = lm;

	m_lookupTable = vtkSmartPointer<vtkLookupTable>::New();
	m_lookupTable->SetNumberOfTableValues(NUMBER_OF_COLOR);
	m_lookupTable->SetTableRange(0.0, 6.0);
	for (int i = 0; i < NUMBER_OF_COLOR; ++i) {
		double rgba[4];
		for (int j = 0; j < 4; ++j) {
			rgba[j] = m_overlayColor[i][j] / 255;
		}
		m_lookupTable->SetTableValue(i, rgba);
	}
	m_lookupTable->Build();

}

IADEOverlay::IADEOverlay(OverlayImageData::itkImageType::Pointer data, QObject * parent)
	:IADEOverlay(parent)
{
	m_data->Graft(data);
}

IADEOverlay::IADEOverlay(OverlayImageData * data, QObject * parent)
{
	m_data->ShallowCopy(data);
}
