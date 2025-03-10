#include <queue>
#include <functional>
#include <unordered_set>
#include "lsh.hpp"

using namespace std;

LSH::LSH(DataSet& dataset_, uint32_t window, uint32_t k, uint32_t L, uint32_t table_size)
: Approximator(dataset_) {

	for (uint32_t i = 0; i < L; i++) {
		auto ht = new HashTable<LshAmplifiedHash>(table_size, new LshAmplifiedHash(dataset_.dim(), window, k));

		for (auto point : dataset) 
			ht->insert(*point);

		htables.push_back(ht);
	}
}

LSH::~LSH() { 
	for (auto ht : htables)
		delete ht;
}


vector< PAIR > 
LSH::kANN(DataPoint& query, uint32_t k, Distance<uint8_t, uint8_t> dist) const{
			
	auto comparator = [](const PAIR t1, const PAIR t2) {
		return t1.second > t2.second;
	};

	priority_queue<PAIR, vector<PAIR>, decltype(comparator)> pq(comparator);
	unordered_set<uint32_t> considered;
	
	// For each hashtable, search for neighbours in the corresponding buckets 
	for (auto ht : htables) {
		for(auto p : ht->bucket(query)) {
			
			auto point = p.second;

			if (considered.find(point->label()) != considered.end())
				continue; 

			double distance = dist(query.data(), point->data());

			pq.push(pair(point->label(), distance));
			considered.insert(point->label());
		}
	}

	vector< PAIR > out;
	while(!pq.empty() && (int)k-- > 0) {
		out.push_back(pq.top());
		pq.pop();
	}

	return out;
}


vector< PAIR > 
LSH::RangeSearch(DataPoint& query, double range, Distance<uint8_t, uint8_t> dist) const {

	unordered_set<uint32_t> considered;
	vector< PAIR > out;

	// For each hashtable, search for neighbours in the corresponding buckets 
	for (auto ht : htables) {
		for(auto p : ht->bucket(query)) {
			
			auto point = p.second;

			if(considered.find(point->label()) != considered.end())
				continue; 

			double distance = dist(query.data(), point->data());

			if(distance < range) {
				out.push_back(pair(point->label(), distance));
				considered.insert(point->label());
			}
		}
	}

	return out;
}

// For Reverse Assignment
vector< PAIR > 
LSH::RangeSearch(Vector<double>& query, double range, Distance<uint8_t, double> dist) const {

	unordered_set<uint32_t> considered;
	vector< PAIR > out;

	// For each hashtable, search for neighbours in the corresponding buckets 
	for (auto ht : htables) {
		for(auto p : ht->bucket(query)) {
			
			auto point = p.second;

			if(considered.find(point->label()) != considered.end())
				continue; 

			double distance = dist(point->data(), query);

			if(distance < range) {
				out.push_back(pair(point->label(), distance));
				considered.insert(point->label());
			}
		}
	}

	return out;
}

