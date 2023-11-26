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
: dataset(dataset_), edges(new vector<DataPoint*>[dataset.size()]), k(k_), dist(dist_) {    

    #pragma omp parallel for
    for (auto point : dataset) {
        #pragma omp parallel for
        for (auto p : approx->kANN(*point, k, dist))
            edges[point->label() - 1].push_back(dataset[p.first - 1]);
    }
}

vector< pair<uint32_t, double> >  Graph::query(Vector<uint8_t>& query, 
                                               uint32_t R, uint32_t T, uint32_t E, uint32_t N) {


    auto comparator = [](const pair<uint32_t, double> t1, const pair<uint32_t, double> t2) {
        return t1.second > t2.second;
    };

    priority_queue<pair<uint32_t, double>, vector<pair<uint32_t, double>>, decltype(comparator)> pq(comparator);
    unordered_set<uint32_t> considered;

    for (uint32_t i = 0, size = dataset.size(); i < R; i++) {
        uint32_t index = Vector<uint32_t>(1, UNIFORM, 0, size - 1)[0];
        auto point = dataset[index];
        auto pedges = edges[point->label() - 1];

        for (uint32_t j = 0; j < T; j++) {
            DataPoint* closest = nullptr;
            double min_dist = DBL_MAX;

            for (uint32_t i = 0; i < E; i++) {
                
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


MRNG::MRNG(DataSet& dataset_,  Approximator* approx, Distance<uint8_t,uint8_t> dist_, uint32_t k, uint32_t overhead)
: dataset(dataset_), edges(new vector<DataPoint*>[dataset.size()]), dist(dist_) {
    

    #pragma omp parallel for
    for(auto x : dataset) {
        printf("%d\n", x->label());
        
        vector<pair<uint32_t, double>> neighbors = approx->kANN(*x, k, dist);
        size_t size = neighbors.size();

        size_t i = 0;
        size_t j = 0;
        auto& pedges = edges[x->label() - 1];

        while(i < size && j < overhead){
            
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

}

vector<pair<uint32_t, double>>  MRNG::query(Vector<uint8_t>& query, uint32_t startID, uint32_t K, uint32_t L){

    auto comparator = [](const pair<DataPoint*, double> t1, const pair<DataPoint*, double> t2) {
        return t1.second > t2.second;
    };

    priority_queue<pair<DataPoint*, double>, vector<pair<DataPoint*, double>>, decltype(comparator)> pq(comparator);
    unordered_set<DataPoint*> checked;
    unordered_set<DataPoint*> inserted;


    DataPoint* start = dataset[startID-1];
    double distance = dist(start->data(),query);
    pq.push(pair(start,distance));
    inserted.insert(start);


    uint32_t i = 1;
    while(i < L){
        // std::cout<<i<<std::endl;
        DataPoint* p = pq.top().first;
        while(checked.find(p) != checked.end()){
            pq.pop();
            // if(pq.size() == 0)std::cout<<"hahahaha"<<std::endl;
            p = pq.top().first;
        }

        checked.insert(p);
        for(auto neighbor : edges[p->label() - 1]){
            if(inserted.find(neighbor) != inserted.end())continue;
            // std::cout<<"Inserting "<<neighbor<<" "<<dist(query,neighbor->data())<<std::endl;
            i++;
            pq.push(pair(neighbor,dist(query,neighbor->data())));
            inserted.insert(neighbor);
        }
    }

    vector<pair<uint32_t, double>> ret;
    for(i = 0; i < K; i++){

        DataPoint* p= pq.top().first;
        double distance = pq.top().second;
        ret.push_back(pair(p->label(),distance));
        pq.pop();
    }

    return ret;
}








