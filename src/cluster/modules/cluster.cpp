
#include <tuple>
#include <vector>
#include "Vector.hpp"
#include "DataSet.hpp"

using namespace std;


class ClusteringAlgorithm{

	private:
		DataSet& dataset;
		uint32_t k;
		double (*metric)(Vector<uint8_t>&, Vector<uint8_t>&);
		vector<Vector<uint8_t>> cluster_centers;


	public:

	ClusteringAlgorithm(DataSet& dataset_, uint32_t k_, double (*metric_)(Vector<uint8_t>&, Vector<uint8_t>&))
	: dataset(dataset_), k(k_), metric(metric_){

		vector<tuple<double,Vector<uint8_t>>> distribution;

		double accumulated = 0;
		for(auto point : dataset){
			double probability = 1./dataset.size();
			accumulated += probability;
			distribution.push_back(make_tuple(accumulated,point->data()));
		}

		random_device rd;
    	mt19937 gen(rd());
    	uniform_real_distribution<double> dist(0.0, 1.0);




		double random_value = dist(gen);

		for(uint32_t i = 0; i < k; i++){


			//Select point randomly, according to distribution
			auto candidate = distribution.begin();
			for(;candidate != distribution.end() && random_value > get<0>(*candidate); 
			     ++candidate);

			if(candidate == distribution.end());//Unexpected Error, exit

			//Found Center through random selection
			Vector<uint8_t> center = get<1>(*candidate);
			cluster_centers.push_back(center);
			distribution.erase(candidate); //Remove

			//Find D(i) for all i and Sum of D(i)^2 (denominator)
			vector<double> D;
			double denominator = 0;
			for(auto pair : distribution){
				
				Vector<uint8_t>& v_i = get<1>(pair);

				double min_dist = metric(v_i, *(cluster_centers.begin()));
				for(auto v : cluster_centers){
					min_dist = min(min_dist, metric(v_i,v));
				}
				D.push_back(min_dist);
				denominator += min_dist*min_dist;
			}



			//Reevaluate distribution
			for(uint32_t i = 0; i < D.size(); i++){
				double enumerator = D[i]*D[i];
				distribution[i] = make_tuple(enumerator/denominator,get<0>(distribution[i]));
			}
		}









	}






};