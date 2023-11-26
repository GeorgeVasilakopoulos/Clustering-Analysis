#include <queue>
#include <functional>
#include <unordered_set>
#include <bitset>
#include "cube.hpp"

using namespace std;

// Helper class for finding vertices to search
class VertexHelper {
    private:
        uint32_t init;
        uint32_t k_;
        uint32_t probes_;
        uint32_t hamming;
        uint32_t max;

        queue<uint32_t> queue_;

    public:
        VertexHelper(uint32_t vertex, uint32_t probes, uint32_t k) : init(vertex), k_(k), probes_(probes + 1), hamming(0), max(1 << k_) { }

        uint32_t next(uint32_t vertex) {
            
            // Queue empty => Finished with vertices within hamming distance "hamming" of the original
            if (queue_.empty()) {
                if (++hamming > k_)
                    return vertex;

                // https://stackoverflow.com/questions/40813022/generate-all-sequences-of-bits-within-hamming-distance-t
                // hamming(u, v) = k ==> u ^ v sets k bits
                // Append new vertices
                for (uint32_t variation = 0; variation < max; variation++) {
                    if (std::bitset<32>(variation ^ init).count() == hamming) 
                        queue_.push(variation);
                }
            }
            
            // If no vertices were added, signal to stop
            if (queue_.empty()) {
                hamming = k_ + 1;
                return vertex;
            }

                       
            uint32_t next = queue_.front();
            queue_.pop();
            probes_--;
            return next;
        }

        bool stop() { return probes_ == 0 || hamming > k_; }
};


Cube::Cube(DataSet& dataset_, uint32_t window, uint32_t k, uint32_t probes_, uint32_t points_)
: Approximator(dataset_), htable(HashTable<CubeHash>(1 << k, new CubeHash(dataset_.dim(), window, k))),
  k_(k), probes(probes_), points(points_) {
    for (auto point : dataset) 
        htable.insert(*point);
}

Cube::~Cube() { }


vector< pair<uint32_t, double> > 
Cube::kANN(DataPoint& query, uint32_t k, Distance<uint8_t, uint8_t> dist) const {
			
	auto comparator = [](const pair<uint32_t, double> t1, const pair<uint32_t, double> t2) {
		return t1.second > t2.second;
	};

	priority_queue<pair<uint32_t, double>, vector<pair<uint32_t, double>>, decltype(comparator)> pq(comparator);
	unordered_set<uint32_t> considered;

    uint32_t vertex = htable.get_hash(query);
    VertexHelper vertices(vertex, probes, k_);
	
    // Search exactly "probes" number of vertices and consider at most "points" number of points
    for (uint32_t i = 0, j = 1; i < points && !vertices.stop(); j++, vertex = vertices.next(vertex)) {
        
        for(auto p : htable.bucket(vertex)) {
            
            auto point = p.second;

            if (considered.find(point->label()) != considered.end())
                continue; 

            double distance = dist(query.data(), point->data());

            pq.push(pair(point->label(), distance));
            considered.insert(point->label());

            if (++i >= points) 
                break;
        }
    }

	vector< pair<uint32_t, double> > out;
	while(!pq.empty() && (int)k-- > 0) {
		out.push_back(pq.top());
		pq.pop();
	}

	return out;
}


vector< pair<uint32_t, double> > 
Cube::RangeSearch(DataPoint& query, double range, Distance<uint8_t, uint8_t> dist) const {
    
    unordered_set<uint32_t> considered;
	vector< pair<uint32_t, double> > out;
    
    uint32_t vertex = htable.get_hash(query);
    VertexHelper vertices(vertex, probes, k_);

    // Search exactly "probes" number of vertices and consider at most "points" number of points
    for (uint32_t i = 0, j = 1; i < points && !vertices.stop(); j++, vertex = vertices.next(vertex)) {
        for(auto p : htable.bucket(query)) {
            
            auto point = p.second;

            if(considered.find(point->label()) != considered.end())
                continue; 

            double distance = dist(query.data(), point->data());

            if(distance < range) {
                out.push_back(pair(point->label(),distance));
                considered.insert(point->label());
            }

            if (++i >= points) 
                break;
        }
    }    

	return out;
}

// Reverse Assignment
vector< pair<uint32_t, double> > 
Cube::RangeSearch(Vector<double>& query, double range, Distance<uint8_t, double> dist) const {
	
    unordered_set<uint32_t> considered;
	vector< pair<uint32_t, double> > out;
    
    uint32_t vertex = htable.get_hash(query);
    VertexHelper vertices(vertex, probes, k_);

    // Search exactly "probes" number of vertices and consider at most "points" number of points
    for (uint32_t i = 0, j = 1; i < points && !vertices.stop(); j++, vertex = vertices.next(vertex)) {
        for(auto p : htable.bucket(query)) {
            
            auto point = p.second;

            if(considered.find(point->label()) != considered.end())
                continue; 

            double distance = dist(point->data(), query);

            if(distance < range) {
                out.push_back(pair(point->label(), distance));
                considered.insert(point->label());
            }
            
            if (++i >= points) 
                break;
        }
    }    

	return out;
}