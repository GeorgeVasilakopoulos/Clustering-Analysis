#pragma once

#include <endian.h>
#include <vector>
#include "Vector.hpp"


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

    public:
        DataSet(std::string path);
        ~DataSet();

        std::vector<DataPoint*>::iterator begin();
        std::vector<DataPoint*>::const_iterator begin() const;
        std::vector<DataPoint*>::iterator end();
        std::vector<DataPoint*>::const_iterator end() const;

};