#include <tuple>
#include <vector>
#include "utils.hpp"
#include "cluster.hpp"




int main() {

    DataSet dataset("train_images", 1000);

    
    Lloyd l(dataset, 10, l2_distance);
    l.apply();


    // Distance<double> d1 = l2_distance;
    // Distance<uint8_t> d2 = l2_distance;  


    // Vector<uint8_t> myvec(2);
    // Vector<uint8_t> myvec2(2);


    // myvec2[0]=1;
    // myvec2[1]=2;

    // printf("%f\n",d2(myvec,myvec2));


    return 0;
}