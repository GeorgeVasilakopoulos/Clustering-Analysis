#include <tuple>
#include <vector>
#include "utils.hpp"
#include "lsh.hpp"
#include "cluster.hpp"




int main() {

    DataSet dataset("train_images");

    LSH mylsh(dataset,10,10,10,1000);

    printf("Defined\n");
    RAssignment l(dataset, 3, &mylsh,l2_distance,l2_distance);
    l.apply();


    printf("done\n");
    Lloyd l2(dataset, 10, l2_distance);
    l2.apply();
    printf("done\n");


    return 0;
}