#pragma once


#include <tuple>
#include <vector>
#include <unordered_set>
#include "Vector.hpp"
#include "utils.hpp"


class Cluster {
	private:
		Vector<float>* center_;
		std::vector<DataPoint*> points;

	public:
		Cluster(uint32_t size) : center_(new Vector<float>(size)) { }
		Cluster(DataPoint* point) : center_(new Vector<float>(point->data())) { }
		~Cluster() { delete center_; }

		uint32_t size() const { return points.size(); }
		void add(DataPoint* point) { points.push_back(point); }
		std::vector<DataPoint*> cluster() { return points; }
		Vector<float>& center() { return *center_; }
};

class Clustering{
	protected:
		DataSet& dataset;
		double (*dist)(Vector<uint8_t>&, Vector<float>&);
		std::vector<Cluster*> clusters;

		double min_dist(DataPoint& point);

	public:
		Clustering(DataSet& dataset_, uint32_t k_, double (*metric_)(Vector<uint8_t>&, Vector<float>&));
		~Clustering();
		std::vector<Vector<double>*> centers() const;

};





// class LloydsAlgorithm: public ClusteringAlgorithm{
// 	private:
// 		std::unordered_set<Vector<uint8_t>*>* clusters;

// 		uint32_t closestCenter(Vector<uint8_t>& v);
// 		Vector<double>* meanVector(std::unordered_set<Vector<uint8_t>*>& set);



// 	public:
// 		LloydsAlgorithm(DataSet& dataset_, uint32_t k_, double (*dist_)(Vector<double>&, Vector<double>&));
// 		void fit() override;
// };