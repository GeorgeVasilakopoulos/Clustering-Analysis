#include <random>

//////////////////
// Consturctors //
//////////////////

template <typename T>
Vector<T>::Vector(uint32_t size_)
: size(size_), data(new T[size_]) {
	for(uint32_t i = 0; i < size; i++)
		data[i] = 0;
}

template <typename T>
Vector<T>::Vector(uint32_t size_, Distribution distr, T a, T b)
: size(size_), data(new T[size_]) {

	switch (distr) {
	case NORMAL:
		this->normal(a, b);
		break;
	case UNIFORM:
	default:
		this->uniform(a, b);
		break;
	}
}


template <typename T>
Vector<T>::Vector(const Vector<T>& v)
: size(v.len()), data(new T[v.len()]){
	for(uint32_t i = 0; i < v.len(); i++)
		data[i] = v[i];
}


template<typename T>
Vector<T>::Vector(std::ifstream& input, uint32_t size_)
: size(size_), data(new uint8_t[size_]) {
	for(uint32_t i = 0; i < size; i++)
		data[i] = input.get();
}


////////////////
// Destructor //
////////////////

template <typename T>
Vector<T>::~Vector() { delete[] data; }


///////////////
// Utilities //
///////////////

template <typename T>
uint32_t Vector<T>::len() const { return size; }

template <typename T>
void Vector<T>::normal(T mean, T std) {

	std::random_device rd{};
	std::mt19937 gen{rd()};
	std::normal_distribution<float> d(mean, std);

	for(uint32_t i = 0; i < size; i++)
		data[i] = d(gen);
}

template <typename T>
void Vector<T>::uniform(T lower, T upper) {

	std::random_device rd{};
	std::mt19937 gen{rd()};
	
	std::uniform_int_distribution<uint32_t>  di(lower, upper);
	std::uniform_real_distribution<float> dr(lower, upper);

	for(uint32_t i = 0; i < size; i++)
		data[i] = std::is_integral<T>::value ? di(gen) : dr(gen);
}


///////////////////////////
// Overloaded Operations //
///////////////////////////

template <typename T>
T& Vector<T>::operator[](uint32_t i) const { return data[i]; }

template <typename T>
Vector<T>& Vector<T>::operator-() const { return *this *= -1; }


template <typename T>
Vector<T> Vector<T>::operator+(const Vector<T>& v) const{
	if (this->size != v.len()) 
        throw std::runtime_error("Exception in Vector Addition operation: Dimensions of vectors must match!\n");

	Vector out(*this);
	for(uint32_t i = 0; i < size; i++)
		out[i] += v[i];

	return out;
}

template <typename T>
template <typename U>
T Vector<T>::operator*(const Vector<U>& v) const {

	if (size != v.len()) 
        throw std::runtime_error("Exception in Dot Product operation: Dimensions of vectors must match!\n");

	T sum = 0;
	for(uint32_t i = 0; i < size; i++)
		sum += data[i] * v[i];

	return sum;
}

template <typename T>
Vector<T>& Vector<T>::operator+=(const T& scalar) {
	for (uint32_t i = 0; i < size; i++)
		data[i] += scalar;

	return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator*=(const T& scalar) {
	for (uint32_t i = 0; i < size; i++)
		data[i] *= scalar;

	return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator/=(const T& scalar) {
	if (scalar == 0) 
        throw std::runtime_error("Exception in Scalar Division operation: Cannot divide by 0!\n");

	for (uint32_t i = 0; i < size; i++)
		data[i] /= scalar;

	return *this;
}