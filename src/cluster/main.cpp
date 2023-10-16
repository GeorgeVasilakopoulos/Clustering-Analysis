#include <tuple>
#include <vector>
#include "utils.hpp"
#include "Cluster.hpp"




int main() {

    DataSet dataset("train_images");

    
    Lloyd l(dataset, 10, l2_distance);
    l.apply();

    return 0;
}