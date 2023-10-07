#include "Vector.hpp"


//Create Vector with zeros
template <typename T>
Vector<T>::Vector(uint32_t size_)
: size(size_), data(new T[size_]) {
	for(uint32_t i = 0; i < size; i++)
		data[i] = 0;
}

template <>
Vector<uint8_t>::Vector(std::ifstream input, uint32_t size_)
: size(size_), data(new uint8_t[size_]) {
	for(uint32_t i = 0; i < size; i++)
		data[i] = input.get();
}

template <typename T>
template <typename U>
Vector<T>::Vector(const Vector<U>& v){
	std::cout<<"Casting!"<<std::endl;
	size = v.len();
	data = new T[size];


	for(uint32_t i = 0; i < size; i++)
		data[i] = v[i];
}

template <typename T>
Vector<T>::~Vector() { 
	std::cout<<"Destroying"<<std::endl;
	delete[] data; 
}



template <typename T>
T& Vector<T>::operator[](uint32_t i) const {
	
	// Check if index out of bounds

	return data[i];
}


template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& v){
	
	// Assert vectors' dimension match
	// Throw error otherwise

	std::cout<<"Assigning!"<<std::endl;
	for(uint32_t i = 0; i < size; i++){
		data[i] = v[i];
	}

	return *this;
}


template <typename T>
Vector<T> Vector<T>::operator+(const Vector<T>& v) const{
	
	// Assert vectors' dimension match
	// Throw error otherwise

	Vector out(*this);
	for(uint32_t i = 0; i < size; i++){
		out[i] += v[i];
	}
	return out;
}

template <typename T>
Vector<T> Vector<T>::operator-(const Vector<T>& v) const{
	
	// Assert vectors' dimension match
	// Throw error otherwise

	Vector out(*this);
	for(uint32_t i = 0; i < size; i++){
		out[i] -= v[i];
	}
	return out;
}

template <typename T>
uint32_t Vector<T>::len() const{
	return size;
}

template <typename T>
uint32_t Vector<T>::operator*(const Vector<T>& v) const {
	// Assert vectors' dimension match
	// Throw error otherwise

	uint32_t sum = 0;
	for(uint32_t i = 0; i < size; i++)
		sum += data[i] * v[i];
	
	return sum;
}

template <typename T, typename U>
Vector<T> operator*(const U& scalar, const Vector<T>& v){
	return v * scalar;
}



template <typename T>
template <typename U>
Vector<T> Vector<T>::operator*(const U& scalar) const {

	std::cout<<"before"<<std::endl;

	// const Vector& bla = *this;
	Vector out = Vector(*this);

	std::cout<<"after"<<std::endl;
	
	printf("%p\n%p\n",out.data, this->data);

	for(uint32_t i = 0; i < size; i++)
		out[i] *= scalar;

	return out;
}


template <typename T>
template <typename U>
Vector<T> Vector<T>::operator/(const U& scalar) const {
	// Check for division by 0

	Vector out(*this);
	for(uint32_t i = 0; i < size; i++)
		out[i] /= scalar;

	return out;
}




//Unary - operator
template <typename T>
Vector<T> Vector<T>::operator-()const{

	Vector out(*this);
	for(uint32_t i = 0; i < size; i++){
		out[i] *= -1;
	}
	return out;

}






