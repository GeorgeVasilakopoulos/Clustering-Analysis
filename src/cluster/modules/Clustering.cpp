// #include <vector>
// #include <unordered_set>
// #include <cfloat>

// #include "Clustering.hpp"


// double Clustering::min_dist(DataPoint& point) {
// 	double min = DBL_MAX;

// 	for (auto cluster : clusters) {
// 		double distance = dist(point.data(), cluster->center());
// 		min = distance < min ? distance : min;
// 	}

// 	return min;
// }


// Clustering::Clustering(DataSet& dataset_, uint32_t k, Distance<double> dist_) 
// : dataset(dataset_), dist(dist_) {
	
// 	bool* chosen = new bool[dataset.size()]();

// 	uint32_t init_center = Vector<uint32_t>(1, UNIFORM, 0, dataset.size() - 1)[0];

// 	clusters.push_back(new Cluster(dataset[init_center]));
// 	chosen[init_center] = true;
	
// 	for (uint32_t i = 1; i < k; i++) {
// 		std::vector<std::pair<uint32_t, double>> distances;
// 		std::vector<std::pair<uint32_t, double>> probs;

// 		double sum = 0;
// 		for (uint32_t j = 0, size = dataset.size(); j < size; j++) {
// 			if (chosen[j])
// 				continue;
			
// 			double distance = min_dist(*dataset[j]);
// 			distance *= distance;

// 			distances.push_back(std::pair(j, distance));
// 			sum += distance;
// 		}

// 		for (auto pair : distances) 
// 			probs.push_back(std::pair(pair.first, pair.second / sum));
		
// 		double prob = Vector<float>(1, UNIFORM, 0, 1)[0];
// 		double accum = 0;

// 		for (auto pair : probs) {
// 			accum += pair.second;

// 			if (prob > accum)
// 				continue;
			
// 			clusters.push_back(new Cluster(dataset[pair.first]));
// 			chosen[pair.first] = true;
// 			break;
// 		}
// 	}

// 	delete [] chosen;
// }


// Clustering::~Clustering() {
// 	for (auto cluster : clusters)
// 		delete cluster;
// };