#include <queue>
#include <tuple>
#include <functional>
#include <unordered_set>
#include "lsh.hpp"

using namespace std;

LSH::LSH(DataSet& dataset_, uint32_t window, uint32_t hash_count, uint32_t L, uint32_t table_size)
: dataset(dataset_) {

	for (uint32_t i = 0; i < L; i++) {
		auto ht = new HashTable<LshAmplifiedHash>(table_size, new LshAmplifiedHash(dataset_.vectorSize(), window, hash_count));

		for (auto point : dataset) 
			ht->insert(*point);

		htables.push_back(ht);
	}
}

LSH::~LSH() { 
	for (auto ht : htables)
		delete ht;
}


vector< tuple<uint32_t, double> > 
LSH::kANN(DataPoint& query, uint32_t k, double (*dist)(Vector<uint8_t>&, Vector<uint8_t>&)){
			
	//Define a custom comparator. Necessary for PQ
	auto comparator = [](const std::tuple<uint32_t, double> t1, const std::tuple<uint32_t, double> t2) {
		return get<1>(t1) > get<1>(t2);	// Maybe >
	};

	//PQ declaration
	priority_queue<tuple<uint32_t, double>, vector<tuple<uint32_t, double>>, decltype(comparator)> knn(comparator);
	unordered_set<uint32_t> k_point_set;
	
	for (auto ht : htables) {
		for(auto pair : ht->bucket(query)) {
			
			auto point = get<1>(pair);

			// If query point is point itself or if point is already contained in the pq
			if(query.label() == point->label() || k_point_set.find(point->label()) != k_point_set.end())
				continue; 

			double distance = dist(query.data(), point->data());

			knn.push(make_tuple(point->label(), distance));
			k_point_set.insert(point->label());
		}
	}

	vector< tuple<uint32_t, double> > out;
	while(!knn.empty() && (int)k-- > 0) {
		out.push_back(knn.top());
		knn.pop();
	}

	return out;
}


vector< tuple<uint32_t, double> > 
LSH::RangeSearch(DataPoint& query, double range, double (*dist)(Vector<uint8_t>&, Vector<uint8_t>&)){
			
	//PQ declaration
	unordered_set<uint32_t> k_point_set;
	vector< tuple<uint32_t, double> > out;


	for (auto ht : htables) {
		for(auto pair : ht->bucket(query)) {
			
			auto point = get<1>(pair);

			//If query point is point itself or if point is already contained in the pq
			if(query.label() == point->label() || k_point_set.find(point->label()) != k_point_set.end())
				continue; 

			// possibly needs rewrite
			double distance = dist(query.data(), point->data());

			if(distance < range) {
				out.push_back(make_tuple(point->label(),distance));
				k_point_set.insert(point->label());
				continue;
			}
		}
	}
	return out;
}












tuple<uint32_t, double>
LSH::ANN(DataPoint& query, double (*dist)(Vector<uint8_t>&, Vector<uint8_t>&)){
	return this->kANN(query,1, dist).front();
}






