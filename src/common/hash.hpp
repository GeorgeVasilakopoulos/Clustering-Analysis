#pragma once

#include "../common/Vector.hpp"

class Hash {
    private:
        Vector<float> v;
        float t;
    public:
        Hash(uint32_t size, uint32_t window) 
        : v(size, NORMAL, 0, 1. / window), t(Vector<float>(1, UNIFORM, 0, 1.)[0]) { }
        ~Hash() {}
        
        template <typename T>
        uint32_t apply(Vector<T>& p) { return std::floor(v * p + t); }
};



class AmplifiedHash {
    private:
        uint32_t hash_count;
        Vector<uint32_t> r;
        std::vector<Hash*> h;
    public:
        AmplifiedHash(uint32_t size, uint32_t window, uint32_t hash_count_)
        : hash_count(hash_count_), r(hash_count, UNIFORM, 0, UINT32_MAX) {
            for (uint32_t i = 0; i < hash_count; i++)
                h.push_back(new Hash(size, window));
        }
        
        ~AmplifiedHash() {            
            for (auto hash : h)
                delete hash;
        }

        template <typename T>
        uint32_t apply(Vector<T>& p) { 
            uint32_t M = (1 << 32) - 5;

            uint32_t sum = 0;

            for (uint32_t i = 0; i < hash_count; i++)
                sum += r[i] * (h[i]->apply(p) % M);

            return sum % M;
        }
};