#pragma once


#include <tuple>
#include <vector>
#include <unordered_set>
#include "Vector.hpp"
#include "utils.hpp"
#include "cluster.hpp"


// class Clustering {
// 	protected:
// 		DataSet& dataset;
// 		Distance<double> dist;
// 		std::vector<Cluster*> clusters;

// 		double min_dist(DataPoint& point);

// 	public:
// 		Clustering(DataSet& dataset_, uint32_t k_, Distance<double> dist);
// 		~Clustering();
// };