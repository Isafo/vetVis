
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

int main(int argc, char *argv[])
{
	char a;
	int winSize[2] = { 300, 300 };

	// create a rendering window and renderer
	vtkSmartPointer<vtkRenderer> ren = vtkRenderer::New();
	vtkSmartPointer<vtkRenderWindow> renWindow = vtkRenderWindow::New();
	renWindow->SetSize(winSize);
	renWindow->AddRenderer(ren);

	vtkSmartPointer<vtkConeSource> cone = vtkConeSource::New();
	cone->SetResolution(8);

	vtkSmartPointer<vtkPolyDataMapper> coneMapper = vtkPolyDataMapper::New();
	coneMapper->SetInputConnection(cone->GetOutputPort());

	vtkSmartPointer<vtkActor> coneActor = vtkActor::New();
	double color[3];
	color[0] = 1.0;
	color[1] = 0.0;
	color[2] = 0.0;
	coneActor->GetProperty()->SetColor(color);
	coneActor->GetProperty()->SetOpacity(0.8);
	coneActor->SetMapper(coneMapper);

	// assign our actor to the renderer
	ren->AddActor(coneActor);

	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkRenderWindowInteractor::New();
	interactor->SetRenderWindow(renWindow);
	vtkSmartPointer<vtkInteractorStyleTrackballActor> style = vtkInteractorStyleTrackballActor::New();
	interactor->SetInteractorStyle(style);
	interactor->Start();

	// draw the resulting scene
	renWindow->Render();

    return 0;
}
