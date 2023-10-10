#pragma once

#include <iostream>
#include <vector>

#include "DataSet.hpp"


template <typename HF>
class HashTable {
    private:
        HF hash;

        // uint32_t is the hash value, used on the qUeRiNg TrIcK 
        std::vector<std::vector<uint32_t, DataPoint*>> buckets;
    public:
    HashTable();
    ~HashTable();
    bool insert(DataPoint* point);
    std::vector<uint32_t, DataPoint*> bucket(DataPoint* point);
};