
#ifndef _OBJECT_HH_
#define _OBJECT_HH_

#include <iostream>

class Object {
public:
  
  virtual void print(std::ostream &out) = 0;
};

#endif

