#pragma once

#include "DataSet.hpp"
#include "lsh_hash.hpp"
#include "Vector.hpp"
#include "HashTable.hpp"
#include <functional>

class LSH{

	private:
		DataSet& dataset;
        std::vector<HashTable*> htables;
		uint32_t table_size;
	public:
		LSH(DataSet& dataset_, uint32_t window, uint32_t hash_count, uint32_t L,uint32_t table_size);
		~LSH();

		std::vector< std::tuple<uint32_t, double> > 
		kNearestNeighbors(DataPoint& p, double (*dist)(Vector<uint8_t>&, Vector<uint8_t>&), uint32_t k);
};