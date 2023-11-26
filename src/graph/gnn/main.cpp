#include <iostream>
#include "GNN.hpp"
#include "lsh.hpp"
#include "cube.hpp"




int main() {

    //  -k 150 -E 50 -R 10 -N 5 και greedy=30
    printf("here1\n");

    DataSet train_dataset("./train_images");
    // DataSet train_dataset("./train_images", 10000);
    printf("here2\n");

    DataSet test_dataset("./test_images", 5);
    printf("here3\n");

    Approximator* approx = new LSH(train_dataset, 2600, 4, 5, train_dataset.size() / 8);
    Approximator* cube   = new Cube(train_dataset, 2600, 7, 10, 6000);
    printf("here4\n");

    // Graph graph = Graph(train_dataset, 150, approx, l2_distance);
    MRNG graph = MRNG(train_dataset, approx, l2_distance, l2_distance, 1000, 500);
    printf("here5\n");


	Stopwatch sw = Stopwatch();

    for (auto point : test_dataset) {
        sw.start();
        // auto vec = graph.query(point->data(), 5, 10, 30, 5);
        auto vec = graph.query(point->data(), 10, 1000);
        // auto vec = newgraph.query(point->data(),15,10,10);
        printf("%d, %f -- %f\n", vec[0].first, vec[0].second, sw.stop());

        sw.start();
        auto vec1 = approx->kANN(*point, 5, l2_distance);
        printf("%d, %f -- %f\n", vec1[0].first, vec1[0].second, sw.stop());
        
        sw.start();
        auto vec2 = cube->kANN(*point, 5, l2_distance);
        printf("%d, %f -- %f\n", vec2[0].first, vec2[0].second, sw.stop());

        sw.start();
        auto vec3 = approx->kNN(*point, 5, l2_distance);
        printf("%d, %f -- %f\n\n", vec3[0].first, vec3[0].second, sw.stop());

    }






    return 0;
}




// #include <iostream>
// #include <set>

// using std::set;


// int main() {

//     set<uint32_t> my_set;

//     my_set.insert(6);
//     my_set.insert(4);
//     my_set.insert(1);
//     my_set.insert(5);
//     my_set.insert(2);
//     my_set.insert(3);

//     for (uint32_t i : my_set)
//         printf("%d\n", i);

//     return 0;
// }