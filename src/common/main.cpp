#include <iostream>

// #include "Vector.hpp"
#include "utils.hpp"
#include <unordered_map>

int main(){


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

	// Vector<float> rv1(20, UNIFORM, 0, 1);
	// // Vector<float> rv2(20,UNIFORM,0,1);
	// // Vector<float> rv3(20,UNIFORM,0,1);
	// // Vector<float> rv4(20,UNIFORM,0,1);

	// AmplifiedHash myhash(20,2,5);
	// std::unordered_map<int, std::vector<Vector<float>>> hashTable;
	// hashTable[myhash.apply(rv1)].push_back(rv1);
	// std::cout << (*(hashTable[myhash.apply(rv1)].begin()))[0] << std::endl;


	return 0;
}