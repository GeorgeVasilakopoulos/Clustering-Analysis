#pragma once

#include <iostream>
#include <fstream>
#include <string>

typedef enum { NORMAL, UNIFORM } Distribution;

template <typename T>
class Vector{

	private:
		uint32_t size;
		T* data;
		
	public:
		Vector(uint32_t size);
		Vector(uint32_t size, Distribution distr, T a, T b);
		Vector(const Vector<T>& v);
		template <typename U>
		Vector(const Vector<U>& v);
		Vector(std::ifstream& input, uint32_t size);
		~Vector();

		uint32_t len() const;
		void normal(T mean, T std);
		void uniform(T lower, T upper);
		std::string asString()const;
		std:: string asDigit()const;

		T& operator[](uint32_t index) const;
		Vector& operator-() const;

		Vector operator+(const Vector& vector) const;
		template <typename U>
		T operator*(const Vector<U>& vector) const;
		template <typename U>
		Vector& operator+=(const Vector<U>& vector);

		Vector& operator+=(const T& scalar);
		Vector& operator*=(const T& scalar);
		Vector& operator/=(const T& scalar);

		T* get();
};

#include "../modules/Vector.tcc"