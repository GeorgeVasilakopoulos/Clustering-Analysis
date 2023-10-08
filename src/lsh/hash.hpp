#pragma once

#include "Vector.hpp"

class Hash {
    private:
        const Vector<float>& v;
        float t;
    public:
        Hash(uint32_t size, uint32_t window);
        
        template <typename T>
        uint32_t apply(Vector<T> vector);
};

class AmplifiedHash {
    private:
        uint32_t hash_count;
        const Vector<uint32_t> r;
        std::vector<Hash*> h;
    public:
        AmplifiedHash(uint32_t size, uint32_t window, uint32_t hash_count);
        ~AmplifiedHash();

        template <typename T>
        uint32_t apply(Vector<T> p);
};