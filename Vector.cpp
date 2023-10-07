#include "Vector.hpp"


Vector::Vector(std::ifstream input, uint32_t size_): 
	size(size_),data(new uint8_t[size_])
{
	for(uint8_t i=0; i < size; i++){
		data[i] = input.get();
	}
}


Vector::~Vector(){
	delete[] data;
}



Vector::Vector(const Vector& v){
	size = v.size;
	data = new uint8_t[v.size];
	for(uint8_t i=0; i < size; i++){
		data[i] = v[i];
	}
}


uint8_t& Vector::operator[](uint32_t i)const{
	return data[i];
}


uint32_t Vector::operator*(const Vector& v)const{
	

	//Check vector sizes


	uint32_t sum = 0;
	for(uint32_t i = 0; i < size; i++){
		sum += data[i] * v[i];
	}
	return sum;
}






Vector Vector::operator*(const uint32_t& scalar)const{

	Vector out(*this);
	for(uint32_t i = 0; i < size; i++){
		out[i] *= scalar;
	}

	return out;
}



Vector Vector::operator/(const uint32_t& scalar)const{
	

	//Check if scalar is 0

	Vector out(*this);
	for(uint32_t i = 0; i < size; i++){
		out[i] /= scalar;
	}

	return out;


}








