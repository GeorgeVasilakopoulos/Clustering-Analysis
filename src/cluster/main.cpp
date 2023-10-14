#include <tuple>
#include <vector>
#include "Vector.hpp"
#include "utils.hpp"
#include "cluster.hpp"


double dist(Vector<uint8_t>& v1, Vector<float>& v2){
    
    if (v1.len() != v2.len()) 
        throw std::runtime_error("Exception in Distance Metric: Dimensions of vectors must match!\n");

    double sum = 0;
    for(uint32_t i = 0; i < v1.len(); i++){
        double diff = (float)v1[i] - (float)v2[i];
        sum += diff * diff; 
    }

    return sqrt(sum);
}

int main() {
    
    DataSet dataset("train_images");
    Clustering algo(dataset, 15, dist);

    // std::vector<Vector<double>*> centers = algo.getClusterCenters();

    // centers[0]->print();

  

    return 0;
}