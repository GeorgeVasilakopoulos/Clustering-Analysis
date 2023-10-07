#pragma once

#include <iostream>
#include <fstream>



class Vector{

private:
	uint8_t* data;
	uint32_t size;
public:


	Vector(uint32_t size_);
	Vector(std::ifstream input, uint32_t size_);
	~Vector();
	Vector(const Vector&);

	Vector operator+(const Vector&) const;
	Vector operator-(const Vector&) const;
	Vector operator-()const;
	uint32_t operator*(const Vector&) const;
	Vector operator*(const uint32_t&) const;
	Vector operator/(const uint32_t&) const;
	uint8_t& operator[](uint32_t)const;

	friend Vector operator*(const uint32_t&, const Vector&);
	
};


