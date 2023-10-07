#include "Vector.hpp"


//Create Vector with zeros
Vector::Vector(uint32_t size_)
: size(size_), data(new uint8_t[size_]) {
	for(uint8_t i = 0; i < size; i++)
		data[i] = 0;
}


Vector::Vector(std::ifstream input, uint32_t size_)
: size(size_), data(new uint8_t[size_]) {
	for(uint8_t i = 0; i < size; i++)
		data[i] = input.get();
}

Vector::~Vector() { 
	delete[] data; 
}

Vector::Vector(const Vector& v){
	size = v.size;
	data = new uint8_t[size];

	for(uint8_t i = 0; i < size; i++)
		data[i] = v[i];
}


uint8_t& Vector::operator[](uint32_t i) const {
	
	// Check if index out of bounds

	return data[i];
}


uint32_t Vector::operator*(const Vector& v) const {
	// Assert vectors' dimension match
	// Throw error otherwise

	uint32_t sum = 0;
	for(uint32_t i = 0; i < size; i++)
		sum += data[i] * v[i];
	
	return sum;
}
Vector operator*(const uint32_t& scalar, const Vector& v){
	return v * scalar;
}

Vector Vector::operator*(const uint32_t& scalar) const {

	Vector out(*this);
	for(uint32_t i = 0; i < size; i++)
		out[i] *= scalar;

	return out;
}



Vector Vector::operator/(const uint32_t& scalar) const {
	// Check for division by 0

	Vector out(*this);
	for(uint32_t i = 0; i < size; i++)
		out[i] /= scalar;

	return out;
}




Vector Vector::operator+(const Vector& v) const{
	
	// Assert vectors' dimension match
	// Throw error otherwise

	Vector out(*this);
	for(uint32_t i = 0; i < size; i++){
		out[i] += v[i];
	}
	return out;
}


Vector Vector::operator-(const Vector& v) const{
	
	// Assert vectors' dimension match
	// Throw error otherwise

	Vector out(*this);
	for(uint32_t i = 0; i < size; i++){
		out[i] -= v[i];
	}
	return out;
}

//Unary - operator
Vector Vector::operator-()const{

	Vector out(*this);
	for(uint32_t i = 0; i < size; i++){
		out[i] *= -1;
	}
	return out;

}






