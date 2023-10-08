#include "DataSet.hpp"


////////////////
// Data Point //
////////////////

DataPoint::DataPoint(std::ifstream& input_, uint32_t size, uint32_t id_) : id(id_), vector(new Vector<uint8_t>(input_, size)) { }
DataPoint::~DataPoint() { delete vector; }

uint32_t DataPoint::label() const { return id; }
Vector<uint8_t>& DataPoint::data() const { return *vector; }


//////////////
// Data Set //
//////////////

DataSet::DataSet(std::string path) {
    // check for failure
    std::ifstream input(path.data(), std::ios::binary);

    uint32_t count, h, w;
    input.read((char*)&count, 4); // Discard magic number
    input.read((char*)&count, 4);
    input.read((char*)&h, 4);
    input.read((char*)&w, 4);
    
    count = be32toh(count);
    h     = be32toh(h);
    w     = be32toh(w);

    for (uint32_t i = 0, size = h * w; i < count; i++) 
        points.push_back(new DataPoint(input, size, i + 1));
}

DataSet::~DataSet() {
    for (auto point : points)
        delete point;
}


std::vector<DataPoint*>::iterator DataSet::begin() { return points.begin(); }
std::vector<DataPoint*>::iterator DataSet::end() { return points.end(); }