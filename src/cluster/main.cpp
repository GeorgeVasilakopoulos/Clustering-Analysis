#include <tuple>
#include <vector>
#include "utils.hpp"
#include "lsh.hpp"
#include "cluster.hpp"




int main() {

    DataSet dataset("train_images");

    // LSH mylsh(dataset, 10, 10, 10, 1000);

    // RAssignment l(dataset, 3, &mylsh, l2_distance, l2_distance);
    // l.apply();

    Lloyd l2(dataset, 10, l2_distance);
    l2.apply();

    Stopwatch sw;
    sw.start();

    printf("Starting silh\n");
    auto sil = l2.silhouettes(l2_distance);
    printf("took %f seconds\n", sw.stop());

    for (uint32_t i = 0; i < 10; i++)
        printf("%f\n", sil[i]);


    return 0;
}