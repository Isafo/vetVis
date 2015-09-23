
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
#include <vtkSTLReader.h>

int main(int argc, char *argv[])
{
	char a;
	int winSize[2] = { 300, 300 };

	// create a rendering window and renderer
	vtkSmartPointer<vtkRenderer> ren = vtkRenderer::New();
	vtkSmartPointer<vtkRenderWindow> renWindow = vtkRenderWindow::New();
	renWindow->SetSize(winSize);
	renWindow->AddRenderer(ren);

	vtkSmartPointer<vtkSTLReader> STLReader = vtkSTLReader::New();
	STLReader->SetFileName("../42400-IDGH.stl");
	STLReader->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkPolyDataMapper::New();
	mapper->SetInputConnection(STLReader->GetOutputPort());

	vtkSmartPointer<vtkActor> actor = vtkActor::New();
	double color[3];
	color[0] = 1.0;
	color[1] = 0.0;
	color[2] = 0.0;
	actor->GetProperty()->SetColor(color);
	actor->GetProperty()->SetOpacity(1.0);
	actor->SetMapper(mapper);

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
