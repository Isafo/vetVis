
#include "Object.h"

#include <iostream>
#include <vector>

class Container {
public:
  
  inline Object& operator[] (int idx){
    return *list[idx];
  }
  
  void add(Object *o);
  
  void print();
  
protected:
  std::vector<Object*> list;
};
