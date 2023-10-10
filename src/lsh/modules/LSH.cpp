#include <queue>
#include <tuple>
#include "lsh.hpp"

using namespace std;

LSH::LSH(DataSet& dataset_, uint32_t window, uint32_t hash_count, uint32_t table_size)
: dataset(dataset_) {

	for (uint32_t i = 0; i < table_size; i++) {
		auto ht = new HashTable<LshAmplifiedHash>();

		for (auto point : dataset) 
			ht->insert(point);

		htables.push_back(ht);
	}

}

LSH::~LSH() { 
	for (auto ht : htables)
		delete ht;
}


vector< tuple<uint32_t, double> > 
LSH::kNearestNeighbors(DataPoint& query, double (*dist)(Vector<uint8_t>&, Vector<uint8_t>&), uint32_t k){
			
	//Define a custom comparator. Necessary for PQ
	auto comparator = [](const std::tuple<uint32_t, double> t1, const std::tuple<uint32_t, double> t2) {
		return get<1>(t1) < get<1>(t2);	// Maybe >
	};

	//PQ declaration
	priority_queue<tuple<uint32_t, double>, vector<tuple<uint32_t, double>>, decltype(comparator)> knn(comparator);

	
	for (auto ht : htables) {
		for(auto point : ht->bucket(&query)) {
			
			// possibly needs rewrite
			double distance = dist(query.data(), point->data());

			if(knn.size() < k) {
				knn.push(make_tuple(point->getID(), distance));
				continue;
			}

			auto tuple = knn.top();
			if(distance < get<1>(tuple)) {
				knn.push(make_tuple(point->getID(), distance));
				knn.pop();
			}
		}
	}

	vector< tuple<uint32_t, double> > out;
	while(!knn.empty()) {
		out.push_back(knn.top());
		knn.pop();
	}

	return out;
}
