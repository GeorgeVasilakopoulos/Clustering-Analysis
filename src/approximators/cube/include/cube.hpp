#pragma once
#include <functional>

#include "utils.hpp"
#include "cube_hash.hpp"
#include "Vector.hpp"
#include "HashTable.hpp"
#include "Approximator.hpp"


class Cube : public Approximator{

	private:
        HashTable<CubeHash> htable;
        uint32_t k_;
        uint32_t probes;
        uint32_t points;

	public:
		Cube(DataSet& dataset_, uint32_t window, uint32_t k, uint32_t probes, uint32_t points);
		~Cube();

		std::vector< std::pair<uint32_t, double> >
		kANN(DataPoint& p, uint32_t k, Distance<uint8_t, uint8_t> dist) const override;

		std::vector< std::pair<uint32_t, double> > 
		RangeSearch(DataPoint& query, double range, Distance<uint8_t, uint8_t> dist) const override;

		std::vector< std::pair<uint32_t, double> > 
		RangeSearch(Vector<double>& query, double range, Distance<uint8_t, double> dist) const override;
};