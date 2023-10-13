#pragma once


#include <tuple>
#include <vector>
#include "Vector.hpp"
#include "DataSet.hpp"






class ClusteringAlgorithm{
	private:
		DataSet& dataset;
		uint32_t k;
		double (*dist)(Vector<uint8_t>&, Vector<uint8_t>&);
		std::vector<Vector<uint8_t>> cluster_centers;

	public:

		ClusteringAlgorithm(DataSet& dataset_, uint32_t k_, double (*metric_)(Vector<uint8_t>&, Vector<uint8_t>&));
		std::vector<Vector<uint8_t>> getClusterCenters()const;

		// virtual void fit();

};




