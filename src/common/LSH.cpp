#include "DataSet.hpp"
#include "hash.hpp"
#include "Vector.hpp"
#include <unordered_map>
#include <queue>

using namespace std;


class LSH{

	private:
		DataSet& dataset;
		AmplifiedHash amplified_hash;
		unordered_map<uint32_t, vector<DataPoint*>> hash_table;
		uint32_t table_size;
	public:


		LSH(DataSet& dataset_, uint32_t window, uint32_t hash_count, uint32_t table_size_)
		: dataset(dataset_), amplified_hash(dataset_.vectorSize(), window, hash_count), table_size(table_size_){
			for(auto point : dataset)
				hash_table[amplified_hash.apply(point->data()) % table_size].push_back(point);
			
		}



		vector<tuple<uint32_t,double>> 
		kNearestNeighbors(DataPoint &p, double (*dist)(Vector<uint8_t>&, Vector<uint8_t>&), uint32_t k){
			
			//Define a custom comparator. Necessary for PQ
			auto comparator = [](const tuple<uint32_t,double> t1, const tuple<uint32_t,double> t2){
				return get<1>(t1) < get<1>(t2);	//Maybe >
			};

			//PQ declaration
			priority_queue<tuple<uint32_t, double>, vector<tuple<uint32_t, double>>, decltype(comparator)> knn(comparator);

			uint32_t bucket_id = amplified_hash.apply(p.data());
			for(auto point : hash_table[bucket_id]) {
				double distance = dist(p.data(),point->data());

				if(knn.size() < k) {
					knn.push(make_tuple(point->getID(),distance));
					continue;
				}

				auto tuple = knn.top();
				if(distance < get<1>(tuple)) {
					knn.push(make_tuple(point->getID(),distance));
					knn.pop();
				}
			}

			vector< tuple<uint32_t, double> > out;
			while(!knn.empty()) {
				out.push_back(knn.top());
				knn.pop();
			}

			return out;
		}

		~LSH(){}

};