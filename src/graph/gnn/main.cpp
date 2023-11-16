#include <iostream>
#include "GNN.hpp"
#include "lsh.hpp"
#include "cube.hpp"




int main() {
    printf("here1\n");
    DataSet dataset("./train_images");
    printf("here2\n");

    Approximator* approx = new LSH(dataset, 2600, 4, 5, dataset.size() / 8);

    printf("here3\n");

    Graph g = Graph(dataset, 50, approx, l2_distance);

	Stopwatch sw = Stopwatch();

    sw.start();
    auto vec = g.query(dataset[0]->data(), 5, 10, 30, 2);
    printf("%d, %f -- %f\n", vec[0].first, vec[0].second, sw.stop());

    sw.start();
    auto vec1 = approx->kANN(*dataset[0], 1, l2_distance);
    printf("%d, %f -- %f\n", vec1[0].first, vec1[0].second, sw.stop());

    sw.start();
    auto vec2 = approx->kNN(*dataset[0], 1, l2_distance);
    printf("%d, %f -- %f\n", vec2[0].first, vec2[0].second, sw.stop());


    return 0;
}