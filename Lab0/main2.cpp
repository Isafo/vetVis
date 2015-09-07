
#include <iostream>

#include "Container.h"

#include "Float.h"
#include "String.h"

int main(int argc, char *argv[]){
  
    Container objects;

    Float f1(0.1);
    Float f2(0.2);
    Float f3(0.3);

    String s1("A = ");
    String s2("B = ");
    String s3("C = ");

    objects.add(&s3);
    objects.add(&f2);
    objects.add(&s1);
    objects.add(&f1);
    objects.add(&s2);
    objects.add(&f1);

    objects.print();
  
  return 0;
}

