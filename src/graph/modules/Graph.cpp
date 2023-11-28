#include "Graph.hpp"

#include <omp.h>
#include <cfloat>
#include <cassert>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <queue>

using std::pair;
using std::vector;
using std::priority_queue;
using std::unordered_map;
using std::unordered_set;
using std::set;
using std::min;

Graph::Graph(DataSet& dataset_, Distance<uint8_t, uint8_t> dist_) 
: dataset(dataset_), edges(new vector<DataPoint*>[dataset.size()]), dist(dist_) { assert(dataset.size() > 0); }

Graph::~Graph() { delete [] edges; }


GNNS::GNNS(DataSet& dataset_, Approximator* approx, Distance<uint8_t, uint8_t> dist_, 
         uint32_t k, uint32_t R_, uint32_t T_, uint32_t E_) 
: Graph(dataset_, dist_), R(R_), T(T_), E(E_) {    

    #pragma omp parallel for
    for (auto point : dataset) {
        #pragma omp parallel for
        for (auto p : approx->kANN(*point, k, dist))
            edges[point->label() - 1].push_back(dataset[p.first - 1]);
    }
}

vector<PAIR>  GNNS::query(Vector<uint8_t>& query, uint32_t N) {

    auto comparator = [](const PAIR t1, const PAIR t2) {
        return t1.second > t2.second;
    };

    priority_queue<PAIR, vector<PAIR>, decltype(comparator)> pq(comparator);
    unordered_set<uint32_t> considered;

    for (uint32_t i = 0, size = dataset.size(); i < R; i++) {
        uint32_t index = Vector<uint32_t>(1, UNIFORM, 0, size - 1)[0];
        auto point = dataset[index];

        double prev = DBL_MAX;
        for (uint32_t j = 0; j < T; j++) {

            auto pedges = edges[point->label() - 1];
            uint32_t size = min((uint32_t)edges[point->label() - 1].size(), E);

            DataPoint* closest = nullptr;
            double min_dist = DBL_MAX;

            for (uint32_t i = 0; i < size; i++) {
                
                auto neighb = pedges[i];

                double distance = dist(query, neighb->data());

                if (distance < min_dist) {
                    closest = neighb;
                    min_dist = distance;
                }

                if (considered.find(neighb->label()) != considered.end())
                    continue; 

                pq.push(pair(point->label(), distance));
                considered.insert(neighb->label());
            }


            if (closest == nullptr || prev <= min_dist)
                break;

            point = closest;
            prev = min_dist;
        }
    }


    vector< PAIR > out;
    for (uint32_t i = 0; !pq.empty() && i < N; i++) {
		out.push_back(pq.top());
		pq.pop();
	}

	return out;
}


MRNG::MRNG(DataSet& dataset_,  Approximator* approx, 
           Distance<uint8_t, uint8_t> dist_, Distance<uint8_t, double> dist_centroid, 
           uint32_t k, uint32_t L_)
: Graph(dataset_, dist_), L(L_) {
    
    size_t overhead = std::max(50U, dataset.size() / 100);

    #pragma omp parallel for
    for(auto x : dataset) {
        
        vector<PAIR> neighbors = approx->kANN(*x, k, dist);
        size_t size = neighbors.size();

        size_t i = 0;
        size_t j = 0;
        auto& pedges = edges[x->label() - 1];

        while(i < size && j < overhead){
            if (neighbors[i].first == x->label()) {
                i++;
                continue;
            }
            
            DataPoint* y = dataset[neighbors[i].first - 1];
            double min_dist = neighbors[i++].second;

            bool insert = true;
            for(auto r : pedges) {
                if(min_dist >= dist(r->data(), y->data())) {
                    insert = false;
                    break;
                }
            }

            if(insert) 
                pedges.push_back(y);
            
            j += insert;
        }
    }

    
	auto centroid = new Vector<double>(dataset[0]->data().len());

	for (auto point : dataset)
		*centroid += point->data();
	
	*centroid /= (double)dataset.size();

    double min_dist = DBL_MAX;
	for(auto point : dataset) {
		double distance = dist_centroid(point->data(), *centroid);
        if (distance < min_dist) {
            min_dist = distance;
            nn_of_centroid = point->label();
        }
	}

	delete centroid;
}

vector<PAIR> MRNG::query(Vector<uint8_t>& query, uint32_t N){

    auto comparator = [](PAIR t1, PAIR t2) {
        return t1.second < t2.second;
    };

    set<PAIR, decltype(comparator)> R(comparator);
    unordered_set<uint32_t> considered;
    unordered_set<uint32_t> inserted;


    R.insert(pair(nn_of_centroid, dist(dataset[nn_of_centroid - 1]->data(), query)));
    inserted.insert(nn_of_centroid);

    while(R.size() < L){
        uint32_t point = 0;

        for (auto p : R) {
            if (considered.find(p.first) == considered.end()) {
                point = p.first;
                break;
            }
        }

        if (point == 0)
            break;

        considered.insert(point);

        for(auto neighbor : edges[point - 1]) {
            if(inserted.find(neighbor->label()) != inserted.end())
                continue;

            R.insert(pair(neighbor->label(), dist(query, neighbor->data())));
            inserted.insert(neighbor->label());
        }
    }

    vector<PAIR> ret;
    for (auto p : R) {
        if ((int)N-- <= 0)
            break;
        
        ret.push_back(p);
    }

    return ret;
}








