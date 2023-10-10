
#include <functional>
#include <vector>
#include "HashTable.hpp"


HashTable::HashTable(uint32_t table_size_, std::function<void*()> hash_constructor)
:table_size(table_size_)
{
	buckets = new std::vector<std::tuple<uint32_t,DataPoint*>>[table_size];
	hash = hash_constructor(); //mind = blown
}


bool HashTable::insert(DataPoint& point){
	uint32_t ID = hash->apply(point.data());
	buckets[ID % table_size].push_back(std::make_tuple(ID,&point)); //qUeRying TrIcK

	return 0; //?
}



std::vector<std::tuple<uint32_t, DataPoint*>>&
HashTable::bucketOf(DataPoint& point){
	uint32_t ID = hash->apply(point.data());
	return buckets[ID % table_size];
}


HashTable::~HashTable(){
	delete[] buckets;
	delete hash;
}