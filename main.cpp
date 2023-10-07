#include "Vector.hpp"
#include <iostream>
#include <cstdio>

int main(){

	Vector<uint32_t> v((uint32_t)3);

	v[0] = 1;
	v[1] = 2;
	v[2] = 3;

	Vector<float> v2 = v * 5;
	std::cout<<(float)v2[0]<<" "<<(float)v2[1]<<" "<<(float)v2[2]<<std::endl;





	return 0;
}