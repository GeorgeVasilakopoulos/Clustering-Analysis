#pragma once
#include <vector>
#include "Vector.hpp"
#include "utils.hpp"
#include "Approximator.hpp"


class Cluster {
	private:
		Vector<double>* center_;
		std::vector<DataPoint*> points_;

	public:
		Cluster(uint32_t size) : center_(new Vector<double>(size)) { }
		Cluster(DataPoint* point) : center_(new Vector<double>(point->data())) { }
		~Cluster() { delete center_; }

		uint32_t size() const { return points_.size(); }
		void add(DataPoint* point) { points_.push_back(point); }
		std::vector<DataPoint*> points() { return points_; }
		Vector<double>& center() { return *center_; }
        void update();
        void clear() { points_.clear(); }
};

class Clusterer {
    protected:
        DataSet& dataset;
        uint32_t k;
		Distance<uint8_t, double> dist;

		std::vector<Cluster*> clusters;

		std::pair<double, Cluster*> closest(DataPoint* point);
    public:
        Clusterer(DataSet& dataset, uint32_t k, Distance<uint8_t, double> dist);
        virtual ~Clusterer();
        
        void clear();
        double minDistBetweenClusters();
        std::vector<Cluster*>& get();
        virtual void apply() = 0;
};

class Lloyd : public Clusterer {
    private:

    public:
        Lloyd(DataSet& dataset, uint32_t k, Distance<uint8_t, double> dist);
        void apply() override;
};

class RAssignment : public Clusterer {
    private:
        Approximator* approx;
		Distance<double, double>  dist;
        double minDistBetweenClusters();
    public:
        RAssignment(DataSet& dataset, uint32_t k, Approximator* approx,
                    Distance<uint8_t, double> dist1, 
		            Distance<double, double>  dist2); 
        void apply() override;
};