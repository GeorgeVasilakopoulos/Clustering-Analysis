#include <iostream>

// #include "Vector.hpp"
#include "DataSet.hpp"
#include "hash.hpp"
#include <unordered_map>

int main(){

	// Vector<uint8_t> v1(3);
	// v1[0] = 1; v1[1] = 2; v1[2] = 3;
	
	// Vector<uint8_t> v2 = v1 *= 2;
	// printf("%d %d %d\n", v2[0], v2[1], v2[2]);


	// Vector<double> v2(3);
	// v2[0] = 1.5; v2[1] = 2.5; v2[2] = 3.5;

	// Vector<double> v3(3);
	// v3[0] = 1.5; v3[1] = 2.5; v3[2] = 3.5;

	// Vector<double> v4(2);

	// v1 += 1;
	// v1 *= 3;

	// v3 /= .5;


	// printf("%d %d %d\n", v1[0], v1[1], v1[2]);
	// printf("Dot Prod: %f\n", v2 * v3);

	// try {
	// 	printf("Dot Prod: %f\n", v2 * v4);
	// }
	// catch (std::runtime_error& e) {
	// 	printf("%s", e.what());
	// }

	// DataSet dataset("train_images");

	// int count = 0;
	// for (auto a : dataset) 
	// 	count += a->data().len();

	// printf("%d\n", count / 28 / 28);

	// uint32_t size = 100000;
	// Vector<double> rv(size, NORMAL, 0, 1.);
	// rv /= 2;
	// // Vector<double> rv(size, NORMAL, 0, 1. / 2);

	// double mean = 0;
	// for (uint32_t i = 0; i < size; i++)
	// 	mean += rv[i];
	// mean /= size;

	// double std = 0;
	// for (uint32_t i = 0; i < size; i++)
	// 	std += (rv[i] - mean) * (rv[i] - mean);
	
	// std /= size;

	// double max = rv[0];
	// double min = rv[0];

	// for (uint32_t i = 1; i < size; i++) {
	// 	max = max > rv[i] ? max : rv[i];
	// 	min = min < rv[i] ? min : rv[i];
	// }

	// printf("mean: %g\n", mean);
	// printf("var: %g\n", std);

	// printf("min: %g\n", min);
	// printf("max: %g\n", max);

	
	// Vector<uint32_t> rv1(size, UNIFORM, 1, 10);

	// uint32_t arr[10];
	// for (uint32_t i = 0; i < 10; i++)
	// 	arr[i] = 0;

	// for (uint32_t i = 0; i < size; i++)
	// 	arr[rv1[i] - 1]++;
	
	// int sum1 = 0;
	// for (uint32_t i = 0; i < 10; i++) {
	// 	printf("%d: %4d\n", i, arr[i]);
	// 	sum1 += arr[i];
	// }

	// printf("sum: %d\n", sum1);

	// Vector<float> rv(size, UNIFORM, 0, 1);

	// float min = rv[0];
	// float max = rv[0];

	// for (uint32_t i = 0; i < size; i++) {
	// 	max = max > rv[i] ? max : rv[i];
	// 	min = min < rv[i] ? min : rv[i];
	// }

	// printf("min: %g\n", min);
	// printf("max: %g\n", max);

	Vector<float> rv1(20, UNIFORM, 0, 1);
	// Vector<float> rv2(20,UNIFORM,0,1);
	// Vector<float> rv3(20,UNIFORM,0,1);
	// Vector<float> rv4(20,UNIFORM,0,1);

	AmplifiedHash myhash(20,2,5);
	std::unordered_map<int, std::vector<Vector<float>>> hashTable;
	hashTable[myhash.apply(rv1)].push_back(rv1);
	std::cout << (*(hashTable[myhash.apply(rv1)].begin()))[0] << std::endl;


	return 0;
}