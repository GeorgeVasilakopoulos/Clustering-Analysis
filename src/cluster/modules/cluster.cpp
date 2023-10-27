#include "cluster.hpp"
#include <unordered_map>
#include <unordered_set>
#include <cfloat>

using namespace std;

/////////////
// Cluster //
/////////////

void Cluster::update() {
	if (points_.size() == 0)
		return ;

	auto sum = new Vector<uint32_t>(center_->len());

	for (auto point : points_)
		*sum += point->data();
	
	delete center_;
	center_ = new Vector<double>(*sum);
	*center_ /= (double)points_.size();

	delete sum;
}

void Cluster::add(DataPoint* point) {

	if (points_.size() > 1)
		*center_ *= (double)points_.size();

	points_.insert(point);

	for (uint32_t i = 0, size = (*center_).len(); i < size; i++)
		(*center_)[i] += point->data()[i];

	*center_ /= (double)points_.size();

}

void Cluster::remove(DataPoint* point) {

	if (points_.size() == 1) {
		points_.erase(point);
		
		for (uint32_t i = 0, size = (*center_).len(); i < size; i++)
			(*center_)[i] = 0;
		
		return ;
	}
	
	*center_ *= (double)points_.size();

	points_.erase(point);

	for (uint32_t i = 0, size = (*center_).len(); i < size; i++)
		(*center_)[i] -= point->data()[i];

	*center_ /= (double)points_.size();
}

///////////////
// Clusterer //
///////////////

Clusterer::Clusterer(DataSet& dataset_, uint32_t k_, Distance<uint8_t, double> dist_) 
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
			
			auto p = closest(dataset[j]);
			double distance = p.first;
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
	

	// for (auto cluster : clusters) {
	// 	for (size_t i = 0; i < cluster->center().len(); i++)
	// 		printf("%3d%s", (int)(cluster->center())[i], ((i + 1) % 28) == 0 ? "\n" : " ");
	// 	printf("\n");
	// }
}

Clusterer::~Clusterer() {
	for (auto cluster : clusters)
		delete cluster;
}

void Clusterer::clear() {
	for (auto cluster : clusters)
		cluster->clear();
}

pair<double, Cluster*> Clusterer::closest(DataPoint* point) {

	if (clusters.size() == 0)
        throw runtime_error("Exception in min_dist: Zero clusters present!\n");

	double min = DBL_MAX;
	Cluster* closest = nullptr;

	for (auto cluster : clusters) {
		double distance = dist(point->data(), cluster->center());

		if (distance < min) {
			min = distance;
			closest = cluster;
		}
	}

	return pair(min, closest);
}

std::vector<Cluster*>& Clusterer::get() { return clusters; }

double average_distance(DataPoint* point, Cluster* cluster, Distance<uint8_t, uint8_t> dist_) {
	double sum = 0;
	size_t count = 0;

	for (auto point2 : cluster->points()) {
		if (point == point2)
			continue;
		
		// sum += fast_l2_distance(point, point2);
		sum += dist_(point->data(), point2->data());
		count++;
	}

	return sum / count;

}

pair<vector<double>, double> Clusterer::silhouettes(Distance<uint8_t, uint8_t> dist_) {
	vector<double> metr;
	double stotal = 0;

	for (auto cluster : clusters) {
		
		double sum = 0.;

		for (auto point : cluster->points()) {

			double min = DBL_MAX;
			Cluster* closest = nullptr;

			for (auto cluster1 : clusters) {
				if (cluster1 == cluster)
					continue; 

				double distance = dist(point->data(), cluster->center());

				if (distance < min) {
					min = distance;
					closest = cluster1;
				}
			}

			double a = average_distance(point, cluster, dist_);
			double b = average_distance(point, closest, dist_);

			sum += (b - a) / max(a, b);
		}
		
		stotal += sum;
		metr.push_back(sum / cluster->size());
	}

	return pair(metr, stotal / dataset.size());
}

////////////
// Lloyd //
///////////

Lloyd::Lloyd(DataSet& dataset, uint32_t k, Distance<uint8_t, double> dist) : Clusterer(dataset, k, dist) { }

void Lloyd::apply() {
	Cluster** indexes = new Cluster*[dataset.size()]();
	
	while (true) {
		uint32_t changes = 0;

		for (auto point : dataset) {
			uint32_t index = point->label() - 1;

			auto p = closest(point);

			if (p.second != indexes[index]) {
				changes++;
				
				if (indexes[index] != nullptr)
					indexes[index]->remove(point);
					
				p.second->add(point);

				indexes[index] = p.second;
			}
		}

		printf("changes: %d\n", changes);
		if (changes == 0)
			break;

		// for (auto cluster : clusters)
		// 	cluster->update();

		// clear();
	}


	delete [] indexes;

	// for (auto cluster : clusters)
	// 	printf("cluster size: %d\n", cluster->size());
	
	// for (auto cluster : clusters) {
	// 	for (size_t i = 0; i < cluster->center().len(); i++)
	// 		printf("%3d%s", (int)(cluster->center())[i], ((i + 1) % 28) == 0 ? "\n" : " ");
	// 	printf("\n");
	// }
}


////////////////////////
// Reverse Assignment //
////////////////////////

RAssignment::RAssignment(DataSet& dataset, uint32_t k, Approximator* approx_, 
						 Distance<uint8_t, double> dist1, 
						 Distance<double, double>  dist2) : Clusterer(dataset, k, dist1), approx(approx_), dist(dist2) { }

RAssignment::~RAssignment() {
	delete approx;
}

double RAssignment::minDistBetweenClusters() {
	double distance = DBL_MAX;

	for(auto cluster1 : clusters) {
		for(auto cluster2 : clusters) {
			if (cluster1 == cluster2) 
				continue;

			distance = min(distance, dist(cluster1->center(), cluster2->center()));
		}
	}

	return distance;
}

#define MAX_ITERS 15
void RAssignment::apply() {

	Cluster** indexes = new Cluster*[dataset.size()]();

	double radius = minDistBetweenClusters() / 2;
	uint8_t iters = 0;

	while (iters++ < MAX_ITERS) {

		uint32_t changes = 0;
		
		for (auto cluster : clusters) {

			for (auto p : approx->RangeSearch(cluster->center(), radius, Clusterer::dist)) {
				
				auto index = p.first - 1;
				auto dist  = p.second;

				auto point = dataset[index];
				auto prev  = indexes[index];
				
				if (prev == nullptr || (
						cluster != prev && 
						dist < Clusterer::dist(point->data(), prev->center())
						)
					) {

					changes++;
					
					if (prev != nullptr)
						// prev->remove(point);
						prev->points().erase(point);
						
					// cluster->add(point);
					cluster->points().insert(point);
					indexes[index] = cluster;
				}
			}
		}

		for (auto cluster : clusters)
			cluster->update();

		printf("\tchanges: %d, radius: %f\n", changes, radius);
		if (changes == 0)
			break;
		
		radius *= 2;
	}

	for (auto point : dataset) {
		if (indexes[point->label() - 1] == nullptr) 
			closest(point).second->add(point);
	}

	for (auto cluster : clusters)
		printf("cluster size: %d\n", cluster->size());

	delete [] indexes;
}