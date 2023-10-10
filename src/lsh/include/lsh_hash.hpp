#pragma once

#include "Vector.hpp"

class LshHash {
    private:
        Vector<float> v;
        float t;
    public:
        LshHash(uint32_t size, uint32_t window) 
        : v(size, NORMAL, 0, 1. / window), t(Vector<float>(1, UNIFORM, 0, 1.)[0]) { }
        ~LshHash() {}
        
        template <typename T>
        uint32_t apply(Vector<T>& p) { return std::floor(v * p + t); }
};



class LshAmplifiedHash {
    private:
        uint32_t hash_count;
        Vector<uint32_t> r;
        std::vector<LshHash*> h;
    public:
        LshAmplifiedHash(uint32_t size, uint32_t window, uint32_t hash_count_)
        : hash_count(hash_count_), r(hash_count, UNIFORM, 0, UINT32_MAX) {
            for (uint32_t i = 0; i < hash_count; i++)
                h.push_back(new LshHash(size, window));
        }
        
        ~LshAmplifiedHash() {            
            for (auto hash : h)
                delete hash;
        }

        template <typename T>
        uint32_t apply(Vector<T>& p) { 
            uint32_t M = UINT32_MAX - 4; // 2^32 - 1 == UINT32_MAX

            uint32_t sum = 0;

            for (uint32_t i = 0; i < hash_count; i++)
                sum += r[i] * h[i]->apply(p) % M;

            return sum % M;
        }
};