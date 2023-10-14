#include <vector>
#include "HashTable.hpp"

template <typename T>
HashTable<T>::HashTable(uint32_t table_size_, T* hash_)
: hash(hash_), table_size(table_size_), buckets(new Bucket[table_size]) { }

template <typename T>
HashTable<T>::~HashTable() { delete hash; delete[] buckets; }

template <typename T>
bool HashTable<T>::insert(DataPoint& point) {
	uint32_t hvalue = hash->apply(point.data());

	// if already exists return false

	buckets[hvalue % table_size].push_back(std::make_tuple(hvalue, &point)); //qUeRying TrIcK

	return true; //?
}

template <typename T>
uint32_t HashTable<T>::get_hash(DataPoint& point) const { return hash->apply(point.data()); }

template <typename T>
Bucket& HashTable<T>::bucket(DataPoint& point) {
	uint32_t hvalue = hash->apply(point.data());
	return buckets[hvalue % table_size];
}