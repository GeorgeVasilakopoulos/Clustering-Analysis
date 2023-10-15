#pragma once
#include <functional>

#include "utils.hpp"
#include "lsh_hash.hpp"
#include "Vector.hpp"
#include "HashTable.hpp"
#include "Approximator.hpp"

class LSH : public Approximator{

	private:
        std::vector<HashTable<LshAmplifiedHash>*> htables;
		uint32_t table_size;
	public:
		LSH(DataSet& dataset_, uint32_t window, uint32_t k, uint32_t L, uint32_t table_size);
		~LSH();

		std::vector< std::pair<uint32_t, double> >
		kANN(DataPoint& p, uint32_t k, Distance<uint8_t> dist) override;

		std::vector< std::pair<uint32_t, double> > 
		RangeSearch(DataPoint& query, double range, Distance<uint8_t> dist) override;
};