#pragma once
#include <vector>
#include "Vector.hpp"
#include "utils.hpp"
#include "Approximator.hpp"


class Cluster {
	private:
		Vector<double>* center_;
		std::vector<DataPoint*> points;

	public:
		Cluster(uint32_t size) : center_(new Vector<double>(size)) { }
		Cluster(DataPoint* point) : center_(new Vector<double>(point->data())) { }
		~Cluster() { delete center_; }

		uint32_t size() const { return points.size(); }
		void add(DataPoint* point) { points.push_back(point); }
		std::vector<DataPoint*> cluster() { return points; }
		Vector<double>& center() { return *center_; }
};

class Clusterer {
    private:
        DataSet& dataset;
        uint32_t k;
		Distance<double> dist;

		std::vector<Cluster*> clusters;

		double min_dist(DataPoint& point);
    public:
        Clusterer(DataSet& dataset, uint32_t k, Distance<double> dist);
        virtual ~Clusterer() { }

        virtual std::vector<Cluster*> apply() = 0;
};

class Lloyd : public Clusterer {
    private:

    public:
        Lloyd(DataSet& dataset, uint32_t k, Distance<double> dist);
        std::vector<Cluster*> apply() override;
};

class RAssignment : public Clusterer {
    private:
        Approximator* approx;
    public:
        RAssignment(DataSet& dataset, uint32_t k, Distance<double> dist, Approximator* approx);
        std::vector<Cluster*> apply() override;
};