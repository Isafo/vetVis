#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyle.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkProperty.h>
#include <vtkStructuredPointsReader.h>
#include <vtkContourFilter.h>
#include <vtkStructuredPoints.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkHedgeHog.h>
#include <vtkStreamLine.h>
#include <vtkPlaneSource.h>
#include <vtkTubeFilter.h>
#include <vtkRungeKutta4.h>


#include "TraceReader.h"

int main(int argc, char *argv[])
{
	int winSize[2] = { 300, 300 };
	bool lines;

	// create a rendering window and renderer
	vtkSmartPointer<vtkRenderer> ren = vtkRenderer::New();
	vtkSmartPointer<vtkRenderWindow> renWindow = vtkRenderWindow::New();
	renWindow->SetSize(winSize);
	renWindow->AddRenderer(ren);

	vtkSmartPointer<vtkPolyData> data = vtkPolyData::New();
	TraceReader tReader;
	tReader.SetFileName("../mob.data");

	std::cout << "Choose to visualise with lines (1) or points (0)\n";
	std::cin >> lines;

	data = tReader.GetOutput(lines);

	vtkSmartPointer<vtkStructuredPoints> points = vtkStructuredPoints::New();
	points->SetDimensions(64, 64, 64);
	points->SetOrigin(-1, -1, -1);
	double spacing = 1.0 / 32.0;
	points->SetSpacing(spacing, spacing, spacing);
	vtkSmartPointer<vtkFloatArray> fArray = vtkFloatArray::New();
	fArray->SetNumberOfComponents(3);

	float B[3];
	B[1] = 0.188;
	B[2] = 0.1;

	for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 64; y++) {
			for (int z = 0; z < 64; z++) {
				B[0] = tanh(-1 + (1 / 32.0) * z);
				fArray->InsertTuple(x + 64 * (y + 64 * z), B);
			}
		}
	}
	points->GetPointData()->SetVectors(fArray);

	vtkSmartPointer<vtkHedgeHog> hedge = vtkHedgeHog::New();
	hedge->SetInputData(points);
	hedge->SetScaleFactor(0.7);
	hedge->Update();

	vtkSmartPointer<vtkStreamLine> stream = vtkStreamLine::New();
	

	vtkSmartPointer<vtkPlaneSource> plane = vtkPlaneSource::New();
	plane->SetOrigin(-1, -1, 0);
	plane->SetPoint1(1, 1, 0);
	plane->SetPoint2(1, 0, 0);
	plane->SetResolution(5, 5);
	plane->Update();

	stream->SetInputData(points);
	stream->SetSourceConnection(plane->GetOutputPort());
	stream->SetStepLength(0.01);
	stream->SetIntegrationStepLength(0.01);
	stream->SetMaximumPropagationTime(100);
	stream->SetNumberOfThreads(1);
	stream->SetIntegrationDirectionToForward();
	stream->VorticityOn();
	stream->Update();

	vtkSmartPointer<vtkTubeFilter> tube = vtkTubeFilter::New();
	tube->SetInputData(stream->GetOutput());
	tube->SetRadius(0.03);
	tube->SetNumberOfSides(10);
	tube->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkPolyDataMapper::New();
	mapper->SetInputData(data);
	mapper->SetScalarRange(0.0, 0.01);
	vtkSmartPointer<vtkPolyDataMapper> mapper2 = vtkPolyDataMapper::New();
	mapper2->SetInputData(hedge->GetOutput());
	vtkSmartPointer<vtkPolyDataMapper> mapper3 = vtkPolyDataMapper::New();
	mapper3->SetInputData(tube->GetOutput());

	vtkSmartPointer<vtkActor> actor = vtkActor::New();
	actor->SetMapper(mapper);
	vtkSmartPointer<vtkActor> actor2 = vtkActor::New();
	actor2->GetProperty()->SetOpacity(0.6);
	actor2->SetMapper(mapper2);
	vtkSmartPointer<vtkActor> actor3 = vtkActor::New();
	actor3->SetMapper(mapper3);
	// assign our actor to the renderer
	ren->AddActor(actor);
	//ren->AddActor(actor2);
	ren->AddActor(actor3);

	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkRenderWindowInteractor::New();
	interactor->SetRenderWindow(renWindow);

	

	vtkSmartPointer<vtkInteractorStyleTrackballActor> style = vtkInteractorStyleTrackballActor::New();
	//interactor->SetInteractorStyle(style);
	interactor->Start();


	// draw the resulting scene
	renWindow->Render();

    return 0;
}
