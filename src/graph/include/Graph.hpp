#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

#include "utils.hpp"
#include "Approximator.hpp"
#include "Vector.hpp"

class Graph {
    protected:
        DataSet& dataset;
        std::vector<DataPoint*>* edges;
        Distance<uint8_t, uint8_t> dist;
    public:
        Graph(DataSet& dataset, Distance<uint8_t, uint8_t> dist);
        virtual ~Graph();

        virtual std::vector<PAIR> query(Vector<uint8_t>& query, uint32_t N) = 0;
};  


class GNN : public Graph{
    private:
        uint32_t R;
        uint32_t T;
        uint32_t E;
    public:
        GNN(DataSet& dataset, Approximator* approx, Distance<uint8_t, uint8_t> dist, 
            uint32_t k, uint32_t R, uint32_t T, uint32_t E);
        std::vector<PAIR> query(Vector<uint8_t>& query, uint32_t N) override;
};  


class MRNG : public Graph {
    private:
        uint32_t nn_of_centroid;
        uint32_t L;
    public:
        MRNG(DataSet& dataset_,  Approximator* approx, 
             Distance<uint8_t, uint8_t> dist, Distance<uint8_t, double> dist_centroid, 
             uint32_t k, uint32_t L);
        std::vector<PAIR> query(Vector<uint8_t>& query, uint32_t N);
};