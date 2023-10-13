#pragma once

#include <iostream>
#include <fstream>

typedef enum { NORMAL, UNIFORM } Distribution;

template <typename T>
class Vector{

	private:
		uint32_t size;
		T* data;
		
	public:
		Vector(uint32_t size);
		Vector(uint32_t size, Distribution distr, T a, T b);
		
		template <typename U>
		Vector(const Vector<U>& v);
		Vector(std::ifstream& input, uint32_t size);
		~Vector();

		uint32_t len() const;
		void normal(T mean, T std);
		void uniform(T lower, T upper);

		T& operator[](uint32_t index) const;
		Vector& operator-() const;

		Vector operator+(const Vector& vector) const;
		template <typename U>
		T operator*(const Vector<U>& vector) const;

		template <typename U>
		Vector& operator=(const Vector<U>& vector);

		void print()const;

		template <typename U>
		Vector& operator+=(const U& scalar);
		
		template <typename U>
		Vector& operator*=(const U& scalar);
		
		template <typename U>
		Vector& operator/=(const U& scalar);
};

#include "../modules/Vector.cpp"