
#include <tuple>
#include <vector>
#include <unordered_set>

#include "Vector.hpp"
#include "cluster.hpp"

#include "utils.hpp"


using namespace std;




ClusteringAlgorithm::ClusteringAlgorithm(DataSet& dataset_, uint32_t k_, double (*dist_)(Vector<double>&, Vector<double>&))
: dataset(dataset_), k(k_), dist(dist_){
	
	vector<tuple<double,Vector<uint8_t>*>> distribution;


	//Uniform distribution across the entire dataset
	double accumulated = 0;
	for(auto point : dataset){
		double probability = 1. / dataset.size();
		accumulated += probability;
		distribution.push_back(make_tuple(accumulated, &(point->data())));
	}

	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<double> unif(0.0, 1.0);

	vector<Vector<uint8_t>> cluster_centers_;


	for(uint32_t i = 0; i < k; i++){
		double random_value = unif(gen);

		//Select point randomly, according to distribution
		auto candidate = distribution.begin();
		for(;candidate != distribution.end() && random_value > get<0>(*candidate); 
		     ++candidate);

		if(candidate == distribution.end());//Unexpected Error, exit
	

		//Push chosen center and remove from distribution
		Vector<uint8_t> center = *get<1>(*candidate);
		cluster_centers_.push_back(center);
		distribution.erase(candidate);


		//Find D(i) for all i and Sum of D(i)^2 (denominator)
		vector<double> D;	//Optimization?
		double denominator = 0;
		for(auto& pair : distribution){
			
			Vector<double> v = *get<1>(pair);

			//Trivial initialization

			Vector<double> myvec = *(cluster_centers_.begin());
			double min_dist = dist(v, myvec);
			
			for(auto& c : cluster_centers_){
				myvec = c;
				min_dist = min(min_dist, dist(v,myvec));
			}
			D.push_back(min_dist);
			denominator += min_dist*min_dist;
		}
			



		//Reevaluate probability distribution
		for(uint32_t i = 0; i < D.size(); i++){
			double enumerator = D[i]*D[i];	//Optimization?
			distribution[i] = make_tuple(enumerator/denominator,get<1>(distribution[i]));
		}
	}
	
	for(auto& v : cluster_centers_){
		cluster_centers.push_back(v);
	}
}

void ClusteringAlgorithm::fit(){}





vector<Vector<double>> ClusteringAlgorithm::getClusterCenters()const{return cluster_centers;}



uint32_t LloydsAlgorithm::closestCenter(Vector<uint8_t>& v){
			
	uint32_t out = 0;
	Vector<double> myvec = v;
	double min_dist = dist(myvec,cluster_centers[0]);


	uint32_t i = 0;
	for(auto c : cluster_centers){
		double distance = dist(myvec,c);
		if(distance < min_dist){
			min_dist = distance;
			out = i;
		}
		i++;
	}
	return out;
}

Vector<double> LloydsAlgorithm::meanVector(unordered_set<Vector<uint8_t>*>& set){
	if(set.empty()){
		//ERROR
	}
	uint32_t vector_size = (*(set.begin()))->len();
	Vector<double> sum(vector_size);
	for(auto v : set){
		sum += *v;
	}
	sum /= set.size();
	return sum;
}


LloydsAlgorithm::LloydsAlgorithm(DataSet& dataset_, uint32_t k_, double (*dist_)(Vector<double>&, Vector<double>&))
:ClusteringAlgorithm(dataset_,k_,dist_){
	clusters = new unordered_set<Vector<uint8_t>*>[k];
}

void LloydsAlgorithm::fit(){
	for(uint32_t counter = 0; counter < 10; counter++){
		for(auto point : dataset){
			clusters[closestCenter(point->data())].insert(&(point->data()));			
		}      

		uint32_t i = 0;
		for(auto& center : cluster_centers){
			center = meanVector(clusters[i]);
			clusters[i].clear();
			i++;
		}
	}
}	