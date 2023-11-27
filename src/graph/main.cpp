#include <iostream>
#include "GNN.hpp"
#include "lsh.hpp"
#include "cube.hpp"



int main() {

    printf("here1\n");

    // DataSet train_dataset("./train_images");
    DataSet train_dataset("./train_images", 100);
    printf("here2\n");

    DataSet test_dataset("./test_images", 5);
    printf("here3\n");

    Approximator* lsh  = new LSH(train_dataset, 2600, 4, 5, train_dataset.size() / 8);
    Approximator* cube = new Cube(train_dataset, 2600, 7, 10, 6000);
    printf("here4\n");

    // GNN graph = GNN(train_dataset, 150, lsh, l2_distance);
    MRNG graph = MRNG(train_dataset, lsh, l2_distance, l2_distance, 1000, 500);
    printf("here5\n");


	Stopwatch sw = Stopwatch();

    for (auto point : test_dataset) {
        sw.start();
        // auto vec = graph.query(point->data(), 5, 10, 30, 5);
        auto vec = graph.query(point->data(), 1000, 10);
        // auto vec = newgraph.query(point->data(),15,10,10);
        printf("%d, %f -- %f\n", vec[0].first, vec[0].second, sw.stop());

        sw.start();
        auto vec1 = lsh->kANN(*point, 5, l2_distance);
        printf("%d, %f -- %f\n", vec1[0].first, vec1[0].second, sw.stop());
        
        sw.start();
        auto vec2 = cube->kANN(*point, 5, l2_distance);
        printf("%d, %f -- %f\n", vec2[0].first, vec2[0].second, sw.stop());

        sw.start();
        auto vec3 = lsh->kNN(*point, 5, l2_distance);
        printf("%d, %f -- %f\n\n", vec3[0].first, vec3[0].second, sw.stop());

    }



    delete lsh;
    delete cube;


    return 0;
}




// #include <iostream>
// #include <fstream>
// #include <queue>
// #include <unordered_set>
// #include <cmath>
// #include <iomanip>

// #include "utils.hpp"
// #include "HashTable.hpp"
// #include "ArgParser.hpp"
// #include "FileParser.hpp"
// #include "cluster.hpp"
// #include "lsh.hpp"
// #include "cube.hpp"

// using namespace std;


// int main(int argc, const char* argv[]) {
// try {
//     ArgParser arg_parser = ArgParser();

//     arg_parser.add("i", STRING);
//     arg_parser.add("c", STRING);
//     arg_parser.add("o", STRING);
//     arg_parser.add("complete", BOOL, "false");
//     arg_parser.add("m", STRING);
//     arg_parser.parse(argc, argv);

//     string input_path;
//     string configuration_path;
//     string out_path;
//     string approx_method;

//     Stopwatch timer;    
//     FileParser file_parser = FileParser();

//     file_parser.add("number_of_clusters",              "k"         );
//     file_parser.add("number_of_vector_hash_tables",    "L",       3);
//     file_parser.add("number_of_vector_hash_functions", "lsh_k",   4);
//     file_parser.add("max_number_M_hypercube",          "M",      10);
//     file_parser.add("number_of_hypercube_dimensions",  "cube_k", 14);
//     file_parser.add("number_of_probes",                "probes",  2);

//     if(arg_parser.parsed("i"))
//         input_path = arg_parser.value<string>("i");
//     else {
//         cout << "Enter path to input file: " << flush;
//         getline(cin, input_path);
//     }


//     if(arg_parser.parsed("c"))
//         configuration_path = arg_parser.value<string>("c");
//     else {
//         cout << "Enter path to configuration file: " << flush;
//         getline(cin, configuration_path);
//     }

//     if(arg_parser.parsed("o"))
//         out_path = arg_parser.value<string>("o");
//     else {
//         cout << "Enter path to output file: " << flush;
//         getline(cin, out_path);
//     }

//     if(arg_parser.parsed("m"))
//         approx_method = arg_parser.value<string>("m");
//     else{
//         cout << "Enter approximator method: " << flush;
//         getline(cin, approx_method);
//         if(approx_method != "Classic" && approx_method != "LSH" && approx_method != "Hypercube")
//             throw runtime_error("Invalid Approximator Method: Valid options are 'Classic', 'LSH' and 'Hypercube'");
//     }

    
//     file_parser.parse(configuration_path);

//     uint32_t k = file_parser.parsed("k") ? file_parser.value("k") : 0, L = file_parser.value("L");
//     uint32_t lsh_k = file_parser.value("lsh_k"), M = file_parser.value("M");
//     uint32_t cube_k = file_parser.value("cube_k"), probes = file_parser.value("probes");
    

//     if(k == 0) {
//         std::string n_of_clusters;
//         cout << "Enter number of clusters: " << flush;
//         getline(cin, n_of_clusters);
//         k = std::stoul(n_of_clusters);
//     }


//     ofstream output_file(out_path, ios::out);
//     if (output_file.fail()) 
//         throw runtime_error(out_path + " could not be opened!\n");

//     cout << "Loading input data... " << flush;
//     timer.start();
//     DataSet dataset(input_path);
//     cout << "Done! (" << std::fixed << std::setprecision(3) << timer.stop() << " seconds)" << endl; 


//     uint32_t window = 2600;
//     uint32_t table_size = dataset.size() / 8;
    
//     cout << "Selecting initial cluster centers... " << flush;
//     timer.start();
//     Clusterer* clusterer = 
//     approx_method == "Classic" ? 
//         (Clusterer*)new Lloyd(dataset, k, l2_distance) :
//         (Clusterer*)new RAssignment(dataset, k, 
//                                     approx_method == "LSH" ? 
//                                         (Approximator*)new LSH(dataset, window, lsh_k, L, table_size) : 
//                                         (Approximator*)new Cube(dataset, window, cube_k, probes, M), 
//                                     l2_distance, l2_distance);
//     cout << "Done! (" << std::fixed << std::setprecision(3) << timer.stop() << " seconds)"<< endl; 
    
    
//     cout << "Applying " << approx_method << " clustering algorithm... "<< flush;
//     timer.start();
//     clusterer->apply();
//     double clustering_time = timer.stop();
//     cout << "Done! (" << std::fixed << std::setprecision(3) << clustering_time << " seconds)"<< endl; 

//     output_file << "Algorithm: ";
//     output_file << ((approx_method=="Classic") ? "Lloyds" : ("Range Search " + approx_method));
//     output_file << '\n';

//     auto clusters = clusterer->get();
//     for(uint32_t i = 0; i < k; i++){
//         output_file << "CLUSTER-" << left << setw(3) << to_string(i + 1);
//         output_file << "{size: " << right << setw(5) << to_string(clusters[i]->size());
//         output_file << ", centroid: " << clusters[i]->center().asString();
//         output_file << "}\n";
//     }

//     output_file << "clustering_time: ";
//     output_file <<  std::fixed << std::setprecision(3) << clustering_time << " sec\n";


//     cout << "Evaluating the Silhouette coefficient... " << flush;
//     timer.start();
//     auto p = clusterer->silhouettes(l2_distance);
//     cout << "Done! (" << std::fixed << std::setprecision(3) << timer.stop() << " seconds)" << endl; 
    

//     auto silhouettes = p.first;
//     auto stotal = p.second;


//     output_file << "Silhouette: [";
//     for (auto score : silhouettes)
//         output_file << std::fixed << std::setprecision(3) << score << ", ";

//     output_file << std::fixed << std::setprecision(3) << stotal << "]\n\n";

//     if (arg_parser.value<bool>("complete")) {
//         for(uint32_t i = 0; i < k; i++){
//             output_file << "CLUSTER-" << left << setw(3) << to_string(i + 1);
//             output_file << "{centroid, ";

//             uint32_t j = 0;
//             uint32_t size = clusters[i]->points().size();
//             for (auto point : clusters[i]->points()) {
//                 if (j++ + 1 >= size)
//                     break;

//                 output_file << right << setw(5) << point->label() << ", ";
//             }

//             output_file << right << setw(5) << (*--clusters[i]->points().end())->label() << "}\n";
//         }
//     }

//     delete clusterer;
// } 
// catch (exception& e) {
//     cerr << e.what();
//     return -1;
// }
//     return 0;
// }