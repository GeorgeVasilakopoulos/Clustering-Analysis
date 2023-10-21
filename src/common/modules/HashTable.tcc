#include <vector>
#include "HashTable.hpp"

template <typename T>
HashTable<T>::HashTable(uint32_t table_size_, T* hash_)
: hash(hash_), table_size(table_size_), buckets(new Bucket[table_size]) { }

template <typename T>
HashTable<T>::~HashTable() { delete hash; delete[] buckets; }

// #include <set>
// static std::set<uint32_t> visited1;
// static int dups = 0;

template <typename T>
bool HashTable<T>::insert(DataPoint& point) {
	uint32_t hvalue = hash->apply(point.data());
	// printf("%14d\n", hvalue);
	// if (visited1.find(hvalue) != visited1.end())
	// 	// std::cout << std::hex << hvalue << "\n";
	// 	printf("dups: %d\n", ++dups);
	// else
	// 	visited1.insert(hvalue);

	// if already exists return false

	buckets[hvalue % table_size].push_back(std::pair(hvalue, &point)); //qUeRying TrIcK

	return true; //?
}

template <typename T>
uint32_t HashTable<T>::get_hash(DataPoint& point) const { return hash->apply(point.data()); }

template <typename T>
Bucket& HashTable<T>::bucket(uint32_t index) { return buckets[index]; }

template <typename T>
Bucket& HashTable<T>::bucket(DataPoint& point) {
	uint32_t hvalue = hash->apply(point.data());
	return buckets[hvalue % table_size];
}

template <typename T>
Bucket& HashTable<T>::bucket(Vector<double>& v) {
	uint32_t hvalue = hash->apply(v);
	return buckets[hvalue % table_size];
}


