#include <tuple>
#include <vector>
#include "Vector.hpp"
#include "DataSet.hpp"
#include "cluster.hpp"


double dist(Vector<uint8_t>& v1, Vector<uint8_t>& v2){
    
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

    // DataSet test("test_images");

    // ClusteringAlgorithm algo(test,2,dist);

    // std::vector<Vector<uint8_t>> centers = algo.getClusterCenters();


    Vector<uint8_t> myv(3);
    myv[0] = 243;
    myv[1] = 3;
    myv[2] = 2;
    

    Vector<double> myd = myv;

    printf("%f %f %f\n",myd[0],myd[1],myd[2]);
    


    return 0;
}