template<typename T>
double l2_distance(Vector<uint8_t>& v1, Vector<T>& v2) {
    if (v1.len() != v2.len()) 
        throw std::runtime_error("Exception in L2 Metric: Dimensions of vectors must match!\n");
    
    //

	double sum = 0;
	for(uint32_t i = 0, size = v1.len(); i < size; i++) {
		double diff = (double)v1[i] - (double)v2[i];
		sum += diff * diff; 
	}

	return sqrt(sum);
}