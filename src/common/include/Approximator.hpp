#pragma once

#include <vector>

#include "utils.hpp"
#include "Vector.hpp"


class Approximator {
    protected:
		DataSet& dataset;

    public:
        Approximator(DataSet& dataset);
        virtual ~Approximator();

        std::vector<PAIR> 
        kNN(DataPoint& query, uint32_t k, Distance<uint8_t, uint8_t> dist);

        virtual std::vector<PAIR>
        kANN(DataPoint& p, uint32_t k, Distance<uint8_t, uint8_t> dist) const = 0;

        virtual std::vector<PAIR> 
        RangeSearch(DataPoint& query, double range, Distance<uint8_t, uint8_t> dist) const = 0;

        virtual std::vector<PAIR> 
        RangeSearch(Vector<double>& query, double range, Distance<uint8_t, double> dist) const = 0;

};