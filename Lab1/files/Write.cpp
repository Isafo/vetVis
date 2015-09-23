#include <vtkImplicitFunction.h>
#include <vtkSampleFunction.h>
#include <vtkStructuredPointsWriter.h>
#include <vtkStructuredPoints.h>
#include <vtkSmartPointer.h>

#include "ImpFunc.h"


int main(int argc, char *argv[])
{

	ImpFunc * function = ImpFunc::New();
	vtkSampleFunction * sample = vtkSampleFunction::New();
	sample->SetImplicitFunction(function);
	sample->SetModelBounds(-6.0, 6.0, -6.0, 6.0, -6.0, 6.0);
	sample->SetSampleDimensions(32, 32, 32);
	sample->ComputeNormalsOff();

	

	vtkSmartPointer<vtkStructuredPoints> points = vtkStructuredPoints::New();
	sample->SetOutput(points);
	sample->Update();

	// write to file
	vtkSmartPointer<vtkStructuredPointsWriter> writer = vtkStructuredPointsWriter::New();

	writer->SetFileName("../hydrogen.vtk");
	writer->SetInputData(points);
	writer->Write();
	return 0;
}
