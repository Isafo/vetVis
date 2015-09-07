
#include "Container.h"

void Container::add(Object* o) {
	list.push_back(o);
}

void Container::print() {
	for (std::vector<Object*>::iterator it = this->list.begin(); it < this->list.end(); it++) {
		(*it)->print(std::cout);
	}
}