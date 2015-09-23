#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyle.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkProperty.h>
#include <vtkStructuredPointsReader.h>
#include <vtkContourFilter.h>
#include <vtkStructuredPoints.h>

int main(int argc, char *argv[])
{
	char a;
	int winSize[2] = { 300, 300 };

	// create a rendering window and renderer
	vtkSmartPointer<vtkRenderer> ren = vtkRenderer::New();
	vtkSmartPointer<vtkRenderWindow> renWindow = vtkRenderWindow::New();
	renWindow->SetSize(winSize);
	renWindow->AddRenderer(ren);

	


	vtkSmartPointer<vtkStructuredPointsReader> pointsReader = vtkStructuredPointsReader::New();
	pointsReader->SetFileName("../hydrogen.vtk");
	
	vtkSmartPointer<vtkStructuredPoints> points = vtkStructuredPoints::New();
	pointsReader->SetOutput(points);
	pointsReader->Update();

	vtkSmartPointer<vtkContourFilter> filter = vtkContourFilter::New();
	filter->SetNumberOfContours(3);
	filter->SetValue(0, 0.001); filter->SetValue(1, 0.002); filter->SetValue(2, 0.003);
	filter->SetInputData(points);

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkPolyDataMapper::New();
	mapper->SetInputConnection(filter->GetOutputPort());
	mapper->SetScalarRange(0.0008, 0.004);

	

	vtkSmartPointer<vtkActor> actor = vtkActor::New();
	actor->SetMapper(mapper);
	
	double color[3] = { 1.0, 0.0, 0.0 };
	actor->GetProperty()->SetOpacity(0.6);
	// assign our actor to the renderer
	ren->AddActor(actor);

	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkRenderWindowInteractor::New();
	interactor->SetRenderWindow(renWindow);
	vtkSmartPointer<vtkInteractorStyleTrackballActor> style = vtkInteractorStyleTrackballActor::New();
	//interactor->SetInteractorStyle(style);
	interactor->Start();


	// draw the resulting scene
	renWindow->Render();

    return 0;
}
