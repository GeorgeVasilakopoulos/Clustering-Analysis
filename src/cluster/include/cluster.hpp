#pragma once


#include <tuple>
#include <vector>
#include <unordered_set>
#include "Vector.hpp"
#include "utils.hpp"






class ClusteringAlgorithm{
	protected:
		DataSet& dataset;
		uint32_t k;
		double (*dist)(Vector<double>&, Vector<double>&);
		std::vector<Vector<double>> cluster_centers;

	public:

		ClusteringAlgorithm(DataSet& dataset_, uint32_t k_, double (*metric_)(Vector<double>&, Vector<double>&));
		std::vector<Vector<double>> getClusterCenters()const;

		virtual void fit();

};





class LloydsAlgorithm: public ClusteringAlgorithm{
	private:
		std::unordered_set<Vector<uint8_t>*>* clusters;

		uint32_t closestCenter(Vector<uint8_t>& v);
		Vector<double> meanVector(std::unordered_set<Vector<uint8_t>*>& set);



	public:
		LloydsAlgorithm(DataSet& dataset_, uint32_t k_, double (*dist_)(Vector<double>&, Vector<double>&));
		void fit() override;
};