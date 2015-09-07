#pragma once
#include "Object.h"

class Float : public Object
{
public:
	Float(float f) { value = f; };
	~Float() {};

	void print(std::ostream &out) { out << value; };

protected:
	float value;
};

