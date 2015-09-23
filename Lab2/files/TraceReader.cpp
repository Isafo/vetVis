#include "TraceReader.h"
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkFloatArray.h>
#include <vtkSmartPointer.h>
#include <vtkIdTypeArray.h>
#include <vtkPointData.h>

    TraceReader::TraceReader(){

    }
    
    TraceReader::~TraceReader(){

    }

    void TraceReader::SetFileName(char* name){
		fileName = name;
    }

    vtkPolyData * TraceReader::GetOutput(bool lines){

		vtkSmartPointer<vtkPolyData> data = vtkPolyData::New();
		vtkSmartPointer<vtkPoints> points = vtkPoints::New();
		vtkSmartPointer<vtkFloatArray> fArray = vtkFloatArray::New();
		fArray->SetNumberOfComponents(1);
		vtkSmartPointer<vtkCellArray> cArray = vtkCellArray::New();
		if (lines)
			cArray->InsertNextCell(1);
		int j = 0;
		FILE * dataFile = fopen(fileName, "r");
		float pos[3];
		float vel[3];
		while (fscanf(dataFile, " %f %f %f %f %f %f \n",
			pos, pos + 1, pos + 2, vel, vel + 1, vel + 2) != EOF) {

			points->InsertNextPoint(pos);
			fArray->InsertValue(j, vel[0] * vel[0]);
			//fArray->InsertNextTuple3(vel[0], vel[1], vel[2]);

			
			if (!lines)
				cArray->InsertNextCell(1);
			cArray->InsertCellPoint(j);
			j++;
		}
		fclose(dataFile);

		data->SetPoints(points);
		//data->GetPointData()->SetVectors(fArray);
		data->GetPointData()->SetScalars(fArray);
		if (lines) {
			cArray->UpdateCellCount(j);
			data->SetLines(cArray);
		}
		else
			data->SetVerts(cArray);

        return data;
    }
