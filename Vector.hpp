
#pragma once

class Vector{


private:
	uint8_t* data;
	uint32_t size;
public:

	Vector(std::ifstream input, uint32_t size);
	~Vector();

	Vector(const Vector&);
	Vector operator+(const Vector&) const;
	Vector operator-(const Vector&) const;
	uint32_t operator*(const Vector&) const;
	Vector operator*(const uint32_t&) const;
	Vector operator/(const uint32_t&) const;
	uint8_t& operator[](uint32_t)const;		

}


