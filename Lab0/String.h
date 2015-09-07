#pragma once
#include "Object.h"
#include <string>

class String : public Object
{
public:
	String(std::string s) { value = s; };
	~String() {};

	void print(std::ostream &out) { out << value; };

protected:
	std::string value;
};