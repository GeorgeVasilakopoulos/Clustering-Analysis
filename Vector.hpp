#pragma once

#include <iostream>
#include <fstream>

#define string std::string 

template <typename T>
class Vector{

private:
	T* data;
	uint32_t size;
public:


	Vector(uint32_t size_);
	Vector(std::ifstream input, uint32_t size_);
	~Vector();

	Vector(const Vector<T>&);
	template <typename U>
	Vector(const Vector<U>&);


	uint32_t len() const;
	void print(string msg = "") const;


	T& operator[](uint32_t)const;
	Vector operator-() const;

	Vector operator+(const Vector&) const;
	Vector operator-(const Vector&) const;
	double operator*(const Vector&) const;

	template <typename U>
	Vector<U> operator*(const U&) const;
	
	template <typename U>
	Vector operator/(const U&) const;
};


#include "Vector.cpp"