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


uint8_t& operator[](uint32_t i){
	return data[i];
}


uint32_t operator*(const Vector& v){
	

	//Check vector sizes


	uint32_t sum = 0;
	for(uint32_t i = 0; i < size; i++){
		sum += data[i] * v[i];
	}
	return sum;
}






Vector operator*(const uint32_t& scalar){

	Vector out(*this);
	for(uint32_t i = 0; i < size; i++){
		out[i] *= scalar;
	}

	return out;
}



Vector operator/(const uint32_t& scalar){
	

	//Check if scalar is 0

	Vector out(*this);
	for(uint32_t i = 0; i < size; i++){
		out[i] /= scalar;
	}

	return out;


}








