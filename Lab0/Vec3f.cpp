#include "Vec3f.h"

#include <algorithm>    // std::copy

Vec3f::Vec3f(){
	value[0] = 0;
	value[1] = 0;
	value[2] = 0; 
};

Vec3f::Vec3f(float x, float y, float z){
	value[0] = x;
	value[1] = y;
	value[2] = z;
}

Vec3f& Vec3f::operator= (const Vec3f &other) {
	if (this != &other) {
		std::copy(other.value, other.value + 3, this->value);
	}
	return *this;
}

Vec3f& Vec3f::operator+ (const Vec3f &other) {

	for (int i = 0; i < 3; i++) {
		this->value[i] += other.value[i];
	}
	return *this;
}

Vec3f& Vec3f::operator- (const Vec3f &other) {

	for (int i = 0; i < 3; i++) {
		this->value[i] -= other.value[i];
	}
	return *this;
}

float Vec3f::operator* (const Vec3f &other) {
	float dotProd;
	
	for (int i = 0; i < 3; i++) {
		dotProd += this->value[i] * other.value[i];
	}
	return dotProd;
}

Vec3f& Vec3f::operator% (const Vec3f &other) {
	Vec3f newVec;

	newVec.value[0] = this->value[1] * other.value[2] - this->value[2] * other.value[1];
	newVec.value[1] = this->value[0] * other.value[2] - this->value[2] * other.value[0];
	newVec.value[2] = this->value[0] * other.value[1] - this->value[1] * other.value[0];

	return newVec;
}

Vec3f operator*(float i, Vec3f v) {
	for (int i = 0; i < 3; i++) {
		v.value[i] = v.value[i] * i;
	}
	return v;
}