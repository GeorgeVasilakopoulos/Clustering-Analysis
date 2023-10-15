#include "Cluster.hpp"

using namespace std;

Clusterer::Clusterer(DataSet& dataset_, uint32_t k_, Distance<double> dist_) 
: dataset(dataset_), k(k_), dist(dist_) { 
    bool* chosen = new bool[dataset.size()]();

	uint32_t init_center = Vector<uint32_t>(1, UNIFORM, 0, dataset.size() - 1)[0];

	clusters.push_back(new Cluster(dataset[init_center]));
	chosen[init_center] = true;
	
	for (uint32_t i = 1; i < k; i++) {
		vector<pair<uint32_t, double>> distances;
		vector<pair<uint32_t, double>> probs;

		double sum = 0;
		for (uint32_t j = 0, size = dataset.size(); j < size; j++) {
			if (chosen[j])
				continue;
			
			double distance = min_dist(*dataset[j]);
			distance *= distance;

			distances.push_back(pair(j, distance));
			sum += distance;
		}

		for (auto p : distances) 
			probs.push_back(pair(p.first, p.second / sum));
		
		double prob = Vector<float>(1, UNIFORM, 0, 1)[0];
		double accum = 0;

		for (auto p : probs) {
			accum += p.second;

			if (prob > accum)
				continue;
			
			clusters.push_back(new Cluster(dataset[p.first]));
			chosen[p.first] = true;
			break;
		}
	}

	delete [] chosen;
}


Lloyd::Lloyd(DataSet& dataset, uint32_t k, Distance<double> dist) 
: Clusterer(dataset, k, dist) { }

vector<Cluster*> Lloyd::apply() {

}


RAssignment::RAssignment(DataSet& dataset, uint32_t k, Distance<double> dist, Approximator* approx_) 
: Clusterer(dataset, k, dist), approx(approx_) { }

vector<Cluster*> RAssignment::apply() {
	
}
