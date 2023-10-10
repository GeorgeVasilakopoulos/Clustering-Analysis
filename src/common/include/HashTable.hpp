#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include "DataSet.hpp"
#include "lsh_hash.hpp"

class HashTable {
    private:
        LshAmplifiedHash* hash;
        uint32_t table_size;
        // uint32_t is the hash value, used on the qUeRiNg TrIcK 
        std::vector<std::tuple<uint32_t, DataPoint*>>* buckets;
    public:
    HashTable(uint32_t table_size_, std::function<void()> hash_constructor);
    ~HashTable();
    bool insert(DataPoint& point);
    std::vector<std::tuple<uint32_t, DataPoint*>>& bucketOf(DataPoint& point);
};