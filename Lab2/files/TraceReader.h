
#ifndef _TraceReader_hh_
#define _TraceReader_hh_

// Forward declarations, real definition (h-file) must be included in c-file
class vtkPoints;
class vtkScalars;
class vtkVectors;
class vtkCellArray;
class vtkPolyData;


// Define my data set reader
// -------------------------

class TraceReader {
public:
    TraceReader();
    ~TraceReader();
    void SetFileName(char* name);
    vtkPolyData * GetOutput(bool lines);

private:
    // Add the variables you need here
	char * fileName;
};

#endif // _TraceReader_hh_
