#include <tuple>
#include <vector>
#include "Vector.hpp"
#include "utils.hpp"
#include "cluster.hpp"


double dist(Vector<double>& v1, Vector<double>& v2){
    
    if (v1.len() != v2.len()) 
        throw std::runtime_error("Exception in Distance Metric: Dimensions of vectors must match!\n");

    double sum = 0;
    for(uint32_t i = 0; i < v1.len(); i++){
        double diff = (double)v1[i] - (double)v2[i];
        sum += diff * diff; 
    }

    return sqrt(sum);
}

int main() {
    
    DataSet test("test_images");
    ClusteringAlgorithm algo(test,2,dist);

    // std::vector<Vector<double>> centers = algo.getClusterCenters();



  

    return 0;
}