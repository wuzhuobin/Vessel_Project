#ifndef __MAXIMUM_WALL_THICKNESS2_H__
#define __MAXIMUM_WALL_THICKNESS2_H__

#include <vtkThreadedImageAlgorithm.h>
#include <memory>
#include <vector>

class MaximumWallThickness2: public vtkThreadedImageAlgorithm
{
public:

	typedef std::vector<std::unique_ptr<double>> WallThicknessVector;

	static MaximumWallThickness2* New();
	vtkTypeMacro(MaximumWallThickness2, vtkThreadedImageAlgorithm);
	virtual void PrintSelf(ostream& os, vtkIndent indent) override;

	vtkGetMacro(VesselWallIntensity, int);
	vtkSetMacro(VesselWallIntensity, int);

	vtkGetMacro(LumenIntensity, int);
	vtkSetMacro(LumenIntensity, int);

protected:

	MaximumWallThickness2();
	~MaximumWallThickness2();

	virtual void ThreadedRequestData(vtkInformation *request,
		vtkInformationVector **inputVector,
		vtkInformationVector *outputVector,
		vtkImageData ***inData, vtkImageData **outData,
		int extent[6], int id) override;



	WallThicknessVector m_wallThicknessVector;
	int VesselWallIntensity = 1;
	int LumenIntensity = 2;

private:

};




#endif // !__MAXIMUM_WALL_THICKNESS2_H__
