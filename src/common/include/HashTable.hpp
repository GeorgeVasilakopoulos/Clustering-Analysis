#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include "utils.hpp"

// uint32_t is the hash value, used on the qUeRiNg TrIcK
typedef std::vector<std::pair<uint32_t, DataPoint*>> Bucket;

template <typename T>
class HashTable {
    private:
        T* hash;
        uint32_t table_size; 
        Bucket* buckets;

    public:
    HashTable(uint32_t table_size, T* hash);
    ~HashTable();

    uint32_t get_hash(DataPoint& point) const;
    bool insert(DataPoint& point);
    Bucket& bucket(uint32_t index);
    Bucket& bucket(DataPoint& point);
    Bucket& bucket(Vector<double>& v);

};

#include "../modules/HashTable.tcc"