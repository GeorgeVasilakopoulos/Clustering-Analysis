#include <iostream>
#include <cstdio>

#include "Vector.hpp"

int main(){

	Vector<uint8_t> v(3);
	v[0] = 1; v[1] = 2; v[2] = 3;
	
	Vector<double> v1(v);
	Vector v2 = v * (uint8_t)2;
	Vector v3 = v1 * 2.1;

	// printf("v : %d, %d, %d\n", v[0], v[1], v[2]);
	// printf("v1: %f, %f, %f\n", (float)v1[0], (float)v1[1], (float)v1[2]);
	// printf("v2: %d, %d, %d\n", v2[0], v2[1], v2[2]);
	// printf("v3: %f, %f, %f\n", (float)v3[0], (float)v3[1], (float)v3[2]);

	v.print("v : ");
	v1.print("v1: ");
	v2.print("v2: ");
	v3.print("v3: ");

	printf("Dot Prod: %f\n", v1 * v3);

	return 0;
}