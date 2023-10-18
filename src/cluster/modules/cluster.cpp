#include "Cluster.hpp"
#include <unordered_map>
#include <cfloat>

using namespace std;

/////////////
// Cluster //
/////////////

void Cluster::update() {
	auto sum = new Vector<uint32_t>(center_->len());

	for (auto point : points_)
		*sum += point->data();
	
	delete center_;
	center_ = new Vector<double>(*sum);
	*center_ /= (double)points_.size();

	delete sum;
}


///////////////
// Clusterer //
///////////////

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


////////////
// Lloyd //
///////////

Lloyd::Lloyd(DataSet& dataset, uint32_t k, Distance<double> dist) : Clusterer(dataset, k, dist) { }

void Lloyd::apply() {
	Cluster** indexes = new Cluster*[dataset.size()]();
	
	while (true) {
		uint32_t changes = 0;

		for (auto point : dataset) {
			uint32_t index = point->label() - 1;

			auto p = closest(point);
			p.second->add(point);

			if (p.second != indexes[index]) {
				changes++;
				indexes[index] = p.second;
			}
		}

		if (changes == 0)
			break;

		for (auto cluster : clusters)
			cluster->update();

		clear();
	}


	delete [] indexes;

	
	// for (auto cluster : clusters) {
	// 	for (size_t i = 0; i < cluster->center().len(); i++)
	// 		printf("%3d%s", (int)(cluster->center())[i], ((i + 1) % 28) == 0 ? "\n" : " ");
	// 	printf("\n");
	// }
}


////////////////////////
// Reverse Assignment //
////////////////////////

RAssignment::RAssignment(DataSet& dataset, uint32_t k, Distance<double> dist, Approximator* approx_) : Clusterer(dataset, k, dist), approx(approx_) { }

//Incomplete
void RAssignment::apply() {

	double radius ; //Initialize

	unordered_map<uint32_t,Cluster*> markings;
	unordered_set<uint32_t> point_set;


	while(/**/){	//Set looping condition
		for(auto cluster : clusters){
			auto pointsInRange = approx.RangeSearchVector(cluster->center(),radius,dist);
			for(auto pair : pointsInRange){
				uint32_t index = pair.first;
				double distance = pair.second;

				if(point_set.find(index) == point_set.end()){
					markings[index] = cluster;
					point_set.insert(index);
					continue;
				}

				if(distance < dist(markings[index]->center(),dataset[index])){
					markings[index] = cluster;
				}

				//To be continued

			}





		}
	}






}
