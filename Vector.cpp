#ifndef BOB
#define BOB
#include "Vector.hpp"

//////////////////
// Consturctors //
//////////////////

template <typename T>
Vector<T>::Vector(uint32_t size_)
: size(size_), data(new T[size_]) {
	for(uint32_t i = 0; i < size; i++)
		data[i] = 0;
}

template<typename T>
Vector<T>::Vector(std::ifstream input, uint32_t size_)
: size(size_), data(new uint8_t[size_]) {
	for(uint32_t i = 0; i < size; i++)
		data[i] = input.get();
}


///////////////////////
// Copy Constructors //
///////////////////////

// Casting
template <typename T>
template <typename U>
Vector<T>::Vector(const Vector<U>& v){
	// std::cout << "Casting!" << std::endl;

	size = v.len();
	data = new T[size];

	for(uint32_t i = 0; i < size; i++)
		data[i] = v[i];
}

// Copying
template <typename T>
Vector<T>::Vector(const Vector<T>& v){
	// std::cout << "Copying!" << std::endl;

	size = v.len();
	data = new T[size];

	for(uint32_t i = 0; i < size; i++)
		data[i] = v[i];
}


////////////////
// Destructor //
////////////////

template <typename T>
Vector<T>::~Vector() { 
	// std::cout << "Destroying" << std::endl;
	delete[] data; 
}


///////////////
// Utilities //
///////////////

template <typename T>
uint32_t Vector<T>::len() const {
	return size;
}

template <typename T>
void Vector<T>::print(string msg) const {
	std::cout << msg;
	
	for (uint32_t i = 0; i < size; i++)
		std::cout << (float)data[i] << " ";
		// printf("%f ", data[i]);

	std::cout << std::endl;
}

///////////////////////////
// Overloaded Operations //
///////////////////////////

template <typename T>
T& Vector<T>::operator[](uint32_t i) const {
	// Check if index out of bounds
	return data[i];
}



template <typename T>
Vector<T> Vector<T>::operator+(const Vector<T>& v) const{
	// Assert vectors' dimension match
	// Throw error otherwise

	Vector out(*this);
	for(uint32_t i = 0; i < size; i++)
		out[i] += v[i];

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
double Vector<T>::operator*(const Vector<T>& v) const {
	// Assert vectors' dimension match
	// Throw error otherwise

	double sum = 0;
	for(uint32_t i = 0; i < size; i++)
		sum += data[i] * v[i];
	
	return sum;
}



template <typename T>
template <typename U>
Vector<U> Vector<T>::operator*(const U& scalar) const {
	Vector<U> out(*this);

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


template <typename T>
Vector<T> Vector<T>::operator-() const {
	Vector out(*this);
	for(uint32_t i = 0; i < size; i++)
		out[i] *= -1;
	
	return out;
}



#endif