#include <queue>
#include <unordered_set>

#include "Approximator.hpp"

using namespace std;


Approximator::Approximator(DataSet& dataset_) : dataset(dataset_) { }; 
Approximator::~Approximator() { }; 

vector<PAIR> 
Approximator::kNN(DataPoint& query, uint32_t k, Distance<uint8_t, uint8_t> dist) {

	auto comparator = [](const PAIR t1, const PAIR t2) {
		return t1.second > t2.second;
	};

	priority_queue<PAIR, vector<PAIR>, decltype(comparator)> pq(comparator);
	
	for(auto point : dataset) {
		double distance = dist(query.data(), point->data());
		pq.push(pair(point->label(), distance));
	}

	vector< PAIR > out;
	
	while(!pq.empty() && (int)k-- > 0) {
		out.push_back(pq.top());
		pq.pop();
	}

	return out;	
}
