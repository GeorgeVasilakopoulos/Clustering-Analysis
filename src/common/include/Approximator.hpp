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

        std::vector<std::pair<uint32_t, double>> 
        kNN(DataPoint& query, uint32_t k, Distance<uint8_t, uint8_t> dist);

        virtual std::vector< std::pair<uint32_t, double> >
        kANN(DataPoint& p, uint32_t k, Distance<uint8_t, uint8_t> dist) = 0;

        virtual std::vector< std::pair<uint32_t, double> > 
        RangeSearch(DataPoint& query, double range, Distance<uint8_t, uint8_t> dist) = 0;

        virtual std::vector< std::pair<uint32_t, double> > 
        RangeSearchVector(Vector<double>& query, double range, Distance<uint8_t, double> dist) = 0;

};