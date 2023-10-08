#include <iostream>

// #include "Vector.hpp"
#include "DataSet.hpp"

int main(){

	Vector<uint8_t> v1(3);
	v1[0] = 1; v1[1] = 2; v1[2] = 3;
	
	Vector<double> v2(3);
	v2[0] = 1.5; v2[1] = 2.5; v2[2] = 3.5;

	Vector<double> v3(3);
	v3[0] = 1.5; v3[1] = 2.5; v3[2] = 3.5;

	Vector<double> v4(2);

	v1 += 1;
	v1 *= 3;

	v3 /= .5;


	printf("%d %d %d\n", v1[0], v1[1], v1[2]);
	printf("Dot Prod: %f\n", v2 * v3);

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

	// uint32_t size = 10000;
	// Vector<double> rv(size);
	// rv.normal(5, 2);

	// double sum = 0;
	// for (uint32_t i = 0; i < size; i++)
	// 	sum += rv[i];

	// printf("mean: %f\n", sum / size);

	
	// Vector<uint32_t> rv1(size);
	// rv1.uniform(1, 10);

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

	return 0;
}