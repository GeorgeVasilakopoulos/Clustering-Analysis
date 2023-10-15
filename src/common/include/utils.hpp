#pragma once

#include <vector>
#include <chrono>
#include "Vector.hpp"

template<typename T>
using Distance = double (*)(Vector<uint8_t>&, Vector<T>&);

// T: uint8_t, float or double
template<typename T>
double l2_distance(Vector<uint8_t>& v1, Vector<T>& v2);


class Stopwatch {
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> time;
    public:
        Stopwatch();
        void start();
        double stop();
};

class DataPoint {
    private:
        const uint32_t id;
        Vector<uint8_t>* vector;
    
    public:
        DataPoint(std::ifstream& input_, uint32_t size, uint32_t id_);
        ~DataPoint();
        uint32_t label() const;
        Vector<uint8_t>& data() const;
};

class DataSet {
    private:
        std::vector<DataPoint*> points;
        uint32_t vector_size;

    public:
        DataSet(std::string path, uint32_t files=0);
        ~DataSet();
        
        uint32_t dim() const;
        uint32_t size() const;
		DataPoint* operator[](uint32_t index) const;

        std::vector<DataPoint*>::iterator begin();
        std::vector<DataPoint*>::iterator end();

};