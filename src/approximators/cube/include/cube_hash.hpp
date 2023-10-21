#pragma once

#include <unordered_map>
#include "lsh_hash.hpp"


class CubeHash {
    private:
        std::vector<LshHash*> lsh;
        std::vector<std::unordered_map<uint32_t, uint8_t>> hash_maps;
        uint32_t k;

    public:
        CubeHash(uint32_t size, uint32_t window, uint32_t k_) : k(k_) {
            for (uint32_t i = 0; i < k; i++) {
                lsh.push_back(new LshHash(size, window));
                hash_maps.push_back(std::unordered_map<uint32_t, uint8_t>());
            }
        }

        ~CubeHash() {
            for (auto hash : lsh)
                delete hash;
        }

        
        template <typename T>
        uint32_t apply(Vector<T>& p) {
            uint32_t value = 0;

            for (uint32_t i = 0; i < k; i++) {
                uint32_t hvalue = lsh[i]->apply(p);

                // If not already computed, sample a value and store it
                if (hash_maps[i].find(hvalue) == hash_maps[i].end()) 
                    hash_maps[i][hvalue] = Vector<uint8_t>(1, UNIFORM, 0, 1)[0];
                
                value = (value << 1) | hash_maps[i][hvalue];
            }

            return value;
        }

};