#include <iostream>

#include "DataSet.hpp"
#include "hash.hpp"
#include <unordered_map>

int main(){

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