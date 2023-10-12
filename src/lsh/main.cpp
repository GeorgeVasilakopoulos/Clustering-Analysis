#include <iostream>
#include <queue>

#include "DataSet.hpp"
#include "HashTable.hpp"
#include "lsh.hpp"
#include <unordered_map>
#include <unordered_set>

using namespace std;

double dist(Vector<uint8_t>& v1, Vector<uint8_t>& v2){
	
	if (v1.len() != v2.len()) 
        throw std::runtime_error("Exception in Distance Metric: Dimensions of vectors must match!\n");

	double sum = 0;
	for(uint32_t i = 0; i < v1.len(); i++){
		double diff = (double)v1[i] - (double)v2[i];
		sum += diff * diff; 
	}

	return sqrt(sum);
}

std::vector<std::tuple<uint32_t, double>> 
kNN(DataSet& dataset, DataPoint& query, uint32_t k, double (*dist)(Vector<uint8_t>&, Vector<uint8_t>&)) {

	auto comparator = [](const std::tuple<uint32_t, double> t1, const std::tuple<uint32_t, double> t2) {
		return get<1>(t1) > get<1>(t2);
	};

	priority_queue<tuple<uint32_t, double>, vector<tuple<uint32_t, double>>, decltype(comparator)> knn(comparator);
	unordered_set<uint32_t> k_point_set;
	
	for(auto point : dataset) {
		if(query.label() == point->label())
			continue; 

		double distance = dist(query.data(), point->data());
		knn.push(std::make_tuple(point->label(), distance));
	}

	std::vector< tuple<uint32_t, double> > out;
	
	while(!knn.empty() && (int)k-- > 0) {
		out.push_back(knn.top());
		knn.pop();
	}

	return out;	
}

vector< tuple<uint32_t, double> > 
RangeSearch(DataSet& dataset, DataPoint& query, double range, double (*dist)(Vector<uint8_t>&, Vector<uint8_t>&)) {

	vector< tuple<uint32_t, double> > out;

	for(auto point : dataset) {

		if(query.label() == point->label())
			continue; 

		double distance = dist(query.data(), point->data());

		if(distance < range)
			out.push_back(make_tuple(point->label(),distance));
	}
	
	return out;
}


int main(){

	DataSet train("train_images");
	DataSet test("test_images");

	LSH haha(train, 5, 4, 5, train.dim() / 8);

	printf("Approximation:\n");
	for(auto i : haha.kANN(*test[0], 10, dist))
		printf("%5u %f\n", std::get<0>(i), std::get<1>(i));
	
	// printf("\nExact:\n");
	// for(auto i : kNN(train, *test[0], 10, dist))
	// 	printf("%5u %f\n", std::get<0>(i), std::get<1>(i));

	// auto myvec = haha.RangeSearch(*mydataset[0], 1300, dist);
	// printf("Range Approximation:\n");
	// for(auto i : myvec)
	// 	printf("%5u %f\n", std::get<0>(i), std::get<1>(i));
	
	// auto myvec1 = RangeSearch(mydataset, *mydataset[0], 1300, dist);
	// printf("\nRange Exact:\n");
	// for(auto i : myvec1)
	// 	printf("%5u %f\n", std::get<0>(i), std::get<1>(i));


	return 0;
}