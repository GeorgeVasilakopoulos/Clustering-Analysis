#include <iostream>
#include <queue>

#include "DataSet.hpp"
#include "HashTable.hpp"
#include "lsh.hpp"
#include <unordered_map>
#include <unordered_set>

using namespace std;

double dist(Vector<uint8_t>&v1, Vector<uint8_t>&v2){
	
	if (v1.len() != v2.len()) 
        throw std::runtime_error("Exception in Distance Metric: Dimensions of vectors must match!\n");

	double sum = 0;
	for(uint32_t i = 0; i < v1.len(); i++){
		double diff = (double)v1[i] - (double)v2[i];
		sum += diff * diff; 
	}

	return sqrt(sum);
}

std::vector<std::tuple<uint32_t,double>> kNN(DataSet& dataset, DataPoint& query, uint32_t k, double (*dist)(Vector<uint8_t>&, Vector<uint8_t>&)){
	//Define a custom comparator. Necessary for PQ
	auto comparator = [](const std::tuple<uint32_t, double> t1, const std::tuple<uint32_t, double> t2) {
		return get<1>(t1) > get<1>(t2);	// Maybe >
	};

	//PQ declaration
	priority_queue<tuple<uint32_t, double>, vector<tuple<uint32_t, double>>, decltype(comparator)> knn(comparator);
	unordered_set<uint32_t> k_point_set;
	

		
	for(auto it=dataset.begin(); it != dataset.end(); it++){



		DataPoint* point = *it;

		//If query point is point itself or if point is already contained in the pq
		if(query.label() == point->label() || !(k_point_set.find(point->label()) == k_point_set.end()))
			continue; 

		// possibly needs rewrite
		double distance = dist(query.data(), point->data());

		printf("Label %d dist %f\n",point->label(),distance);

		if(knn.size() < k) {
			knn.push(std::make_tuple(point->label(), distance));
			k_point_set.insert(point->label());
			continue;
		}

		auto tuple = knn.top();
		if(distance < get<1>(tuple)) {
			uint32_t front_point = get<0>(tuple);
			k_point_set.erase(front_point);
			knn.pop();
			knn.push(make_tuple(point->label(), distance));
			k_point_set.insert(point->label());
		}
	}

	std::vector< tuple<uint32_t, double> > out;
	while(!knn.empty()) {
		out.push_back(knn.top());
		knn.pop();
	}

	return out;	
}








int main(){

	DataSet mydataset("train.idx3-ubyte");
	LSH haha(mydataset,20,10,10,1000);

	auto myvec = haha.kANN(**(mydataset.begin()),10,dist);
	printf("Approximations\n");
	for(auto i : myvec){
		printf("%u %f\n",std::get<0>(i),std::get<1>(i));

	}
	
	myvec = kNN(mydataset,**(mydataset.begin()),10,dist);
	printf("Actuals\n");
	for(auto i : myvec){
		printf("%u %f\n",std::get<0>(i),std::get<1>(i));

	}



	return 0;
}