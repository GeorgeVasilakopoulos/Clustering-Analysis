#include "GNN.hpp"

#include <omp.h>
#include <cfloat>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using std::pair;
using std::vector;
using std::priority_queue;
using std::unordered_map;
using std::unordered_set;

Graph::Graph(DataSet& dataset_, uint32_t k_, Approximator* approx, Distance<uint8_t, uint8_t> dist_) 
: dataset(dataset_), k(k_), dist(dist_) {    

    #pragma omp parallel for
    for (auto point : dataset) {
        #pragma omp parallel for
        for (auto p : approx->kANN(*point, k, dist))
            edges[point].push_back(dataset[p.first]);
    }
}

vector< pair<uint32_t, double> >  Graph::query(Vector<uint8_t>& query, 
                                               uint32_t R, uint32_t T, uint32_t E, uint32_t N) {

    // return vector<pair<uint32_t, double>>();
    auto comparator = [](const pair<uint32_t, double> t1, const pair<uint32_t, double> t2) {
        return t1.second > t2.second;
    };

    priority_queue<pair<uint32_t, double>, vector<pair<uint32_t, double>>, decltype(comparator)> pq(comparator);
    unordered_set<uint32_t> considered;

    for (uint32_t i = 0, size = dataset.size(); i < R; i++) {
        uint32_t index = Vector<uint32_t>(1, UNIFORM, 0, size - 1)[0];
        auto point = dataset[index];

        for (uint32_t j = 0; j < T; j++) {
            DataPoint* closest = nullptr;
            double min_dist = DBL_MAX;

            for (uint32_t i = 0; i < E; i++) {
                
                auto neighb = edges[point][i];

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

            if (!closest)
                break;

            point = closest;
        }
    }

    vector< pair<uint32_t, double> > out;
    for (uint32_t i = 0; !pq.empty() && i < N; i++) {
		out.push_back(pq.top());
		pq.pop();
	}

	return out;
}