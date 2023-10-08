#include "hash.hpp"


//////////
// Hash //
//////////

Hash::Hash(uint32_t size, uint32_t window) 
: v(Vector<float>(size, NORMAL, 0, 1) /= window), t(Vector<float>(1, UNIFORM, 0, window)[0] / window) { }

template <typename T>
uint32_t Hash::apply(Vector<T> p) { return std::floor(v * p + t); }


////////////////////
// Amplified Hash //
////////////////////

AmplifiedHash::AmplifiedHash(uint32_t size, uint32_t window, uint32_t hash_count_) 
: hash_count(hash_count_), r(Vector<uint32_t>(hash_count, UNIFORM, 0, UINT32_MAX)) {
    for (uint32_t i = 0; i < hash_count; i++)
        h.push_back(new Hash(size, window));
}

AmplifiedHash::~AmplifiedHash() {
    for (auto hash : h)
        delete hash;
}

template <typename T>
uint32_t AmplifiedHash::apply(Vector<T> p) { 
    uint32_t M = (1 << 32) - 5;

    uint32_t sum = 0;

    for (uint32_t i = 0; i < hash_count; i++)
        sum += r[i] * h[i]->apply(p) % M;

    return sum % M;
}