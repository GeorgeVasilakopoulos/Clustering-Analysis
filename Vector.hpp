#pragma once

#include <iostream>
#include <fstream>


template <typename T>
class Vector{

private:
	T* data;
	uint32_t size;
public:


	Vector(uint32_t size_);
	Vector(std::ifstream input, uint32_t size_);
	~Vector();



	template <typename U>
	Vector(const Vector<U>&);

	T& operator[](uint32_t)const;
	Vector operator+(const Vector&) const;
	Vector operator-(const Vector&) const;
	Vector& operator=(const Vector&);
	
	uint32_t len() const;

	Vector operator-()const;
	uint32_t operator*(const Vector&) const;

	template <typename U>
	Vector operator*(const U&) const;
	
	template <typename U>
	Vector operator/(const U&) const;


};


#include "Vector.cpp"