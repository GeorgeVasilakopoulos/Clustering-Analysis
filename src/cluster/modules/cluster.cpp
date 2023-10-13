
#include <tuple>
#include <vector>
#include <unordered_set>

#include "Vector.hpp"

#include "DataSet.hpp"
#include "cluster.hpp"

#include "utils.hpp"


using namespace std;




ClusteringAlgorithm::ClusteringAlgorithm(DataSet& dataset_, uint32_t k_, double (*dist_)(Vector<uint8_t>&, Vector<uint8_t>&))
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




	for(uint32_t i = 0; i < k; i++){

		double random_value = unif(gen);

		//Select point randomly, according to distribution
		auto candidate = distribution.begin();
		for(;candidate != distribution.end() && random_value > get<0>(*candidate); 
		     ++candidate);

		if(candidate == distribution.end());//Unexpected Error, exit


		//Push chosen center and remove from distribution
		Vector<uint8_t> center = *get<1>(*candidate);
		cluster_centers.push_back(center);
		distribution.erase(candidate);


		//Find D(i) for all i and Sum of D(i)^2 (denominator)
		vector<double> D;	//Optimization?
		double denominator = 0;
		for(auto pair : distribution){
			
			Vector<uint8_t>& v = *get<1>(pair);

			//Trivial initialization
			double min_dist = dist(v, *(cluster_centers.begin()));
			

			for(auto c : cluster_centers){
				min_dist = min(min_dist, dist(v,c));
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
}







vector<Vector<uint8_t>> ClusteringAlgorithm::getClusterCenters()const{return cluster_centers;}




// class LloydsAlgorithm: public ClusteringAlgorithm{
// 	private:
		

// 		uint32_t closestCenter(Vector& v){
			
// 			uint32_t out = 0;
// 			double min_dist = dist(v,cluster_center[0]);

// 			uint32_t i = 0;
// 			for(auto c : cluster_centers){
// 				double distance = dist(v,c);
// 				if(distance < min_dist){
// 					min_dist = distance;
// 					out = i;
// 				}
// 				i++;
// 			}
// 			return out;
// 		}

// 		uint32_t meanVector(unordered_set<Vector&>* set){



// 		}




// 	public:

// 		LloydsAlgorithm(DataSet& dataset_, uint32_t k_, double (*dist_)(Vector<uint8_t>&, Vector<uint8_t>&))
// 		:ClusteringAlgorithm(dataset_,k_,dist_){
// 			clusters = new unordered_set<Vector*>[k];
// 		}



// 		void fit() override {
// 			unordered_set<Vector&>* clusters;

// 			for(auto point : dataset){
// 				clusters[closestCenter(point.data())].insert(point.data());			
// 			}      

// 			uint32_t i = 0;
// 			for(auto center& : cluster_centers){


// 				i++;
// 			}


// 	    }	
// }