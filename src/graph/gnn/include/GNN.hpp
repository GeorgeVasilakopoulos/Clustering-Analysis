#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

#include "utils.hpp"
#include "Approximator.hpp"
#include "Vector.hpp"



class Graph {
    private:
        DataSet& dataset;
        std::unordered_map<DataPoint*, std::vector<DataPoint*>> edges;
        uint32_t k;
        Distance<uint8_t, uint8_t> dist;
    public:
        Graph(DataSet& dataset, uint32_t k, Approximator* approx, Distance<uint8_t, uint8_t> dist);
        std::vector< std::pair<uint32_t, double> >  query(Vector<uint8_t>& query, 
                                                          uint32_t R, uint32_t T, uint32_t E, uint32_t N);
};  


class MRNG{
private:
    DataSet& dataset;
    std::unordered_map<DataPoint*, std::vector<DataPoint*>> edges;
    Distance<uint8_t, uint8_t> dist;


public:
    MRNG(DataSet& dataset_,  Approximator* approx, Distance<uint8_t,uint8_t> dist_, uint32_t k, uint32_t overhead);
    std::vector<std::pair<uint32_t, double>>  query(Vector<uint8_t>& query, uint32_t startID, uint32_t K, uint32_t L);



};