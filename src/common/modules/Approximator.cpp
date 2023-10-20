#include <queue>
#include <unordered_set>

#include "Approximator.hpp"

using namespace std;


Approximator::Approximator(DataSet& dataset_) : dataset(dataset_) { }; 
Approximator::~Approximator() { }; 

vector<pair<uint32_t, double>> 
Approximator::kNN(DataPoint& query, uint32_t k, Distance<uint8_t, uint8_t> dist) {

	auto comparator = [](const pair<uint32_t, double> t1, const pair<uint32_t, double> t2) {
		return t1.second > t2.second;
	};

	priority_queue<pair<uint32_t, double>, vector<pair<uint32_t, double>>, decltype(comparator)> pq(comparator);
	
	for(auto point : dataset) {
		if(query.label() == point->label())
			continue; 

		double distance = dist(query.data(), point->data());
		pq.push(pair(point->label(), distance));
	}

	vector< pair<uint32_t, double> > out;
	
	while(!pq.empty() && (int)k-- > 0) {
		out.push_back(pq.top());
		pq.pop();
	}

	return out;	
}
