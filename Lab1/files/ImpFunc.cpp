
#include "ImpFunc.h"
#define _USE_MATH_DEFINES 
#include <math.h>
#include <vtkSmartPointer.h>

//------------------------------------------------------------------------------
ImpFunc * ImpFunc::New()
{
    // Skip factory stuff - create class
    return new ImpFunc;
}


// Create the function
ImpFunc::ImpFunc()
{
	Z = 1; a0 = 1; n = 2; l = 1; m = 0;
}

// Evaluate function
double ImpFunc::EvaluateFunction(double x[3])
{
	// To spherical coordinates
	r = sqrt(pow(x[0], 2) + pow(x[1], 2) + pow(x[2], 2));
	theta = acos(x[2] / r);
	phi = atan(x[1] / x[0]);

	// Function time - 0.353553
	//double Rfunc_1 = (pow((Z / (2 * a0)), 1.5)) / sqrt(3);
	//double Rfunc_2 = (Z*r / a0);
	//double Rfunc_3 = exp(-(Z*r / 2 * a0));

	//double Rfunc = Rfunc_1 * Rfunc_2 * Rfunc_3;
	//double Hfunc = sqrt(3 / (4 * M_PI)) * cos(theta);
	//double Cfunc = Rfunc * Hfunc;

	double Cfunc = r * exp(-(r / 2))*cos(theta) / (4 * sqrt(2*M_PI));

	// Not necessary to calculate abs
	double density = pow(abs(Cfunc), 2);

    return density;    // the value of the function
}

// Evaluate gradient for function
void ImpFunc::EvaluateGradient(double x[3], double n[3])
{
    // Not use and thus not needed
}

void ImpFunc::PrintSelf(ostream& os, vtkIndent indent)
{
    vtkImplicitFunction::PrintSelf(os, indent);
}
