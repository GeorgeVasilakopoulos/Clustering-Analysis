#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_set>
#include <cmath>

#include "utils.hpp"
#include "HashTable.hpp"
#include "ArgParser.hpp"
#include "FileParser.hpp"
#include "cluster.hpp"
#include "lsh.hpp"
#include "cube.hpp"

using namespace std;


int main(int argc, const char* argv[]) {
try {
    ArgParser parser = ArgParser();

    parser.add("i", STRING);
    parser.add("c", STRING);
    parser.add("o", STRING);
    parser.add("complete", BOOL);
    parser.add("m", STRING, "5");
    parser.parse(argc, argv);

    string input_path;
    string configuration_path;
    string out_path;
    string approx_method;

    uint32_t k = 10, L = 5;
    uint32_t lsh_k = 4, M = 6000;
    uint32_t cube_k = 14, probes = 10;

    if(parser.parsed("i"))
        input_path = parser.value<string>("i");
    else {
        cout << "Enter path to input file: ";
        getline(cin, input_path);
    }


    if(parser.parsed("c"))
        configuration_path = parser.value<string>("c");
    else {
        cout << "Enter path to configuration file: ";
        getline(cin, configuration_path);
    }

    if(parser.parsed("o"))
        out_path = parser.value<string>("o");
    else {
        cout << "Enter path to output file: ";
        getline(cin, out_path);
    }

    if(parser.parsed("m"))
        approx_method = parser.value<string>("m");
    else{
        cout << "Enter approximator method: ";
        getline(cin, approx_method);
        if(approx_method != "Classic" && approx_method != "LSH" && approx_method != "Hypercube")
            throw runtime_error("Invalid Approximator Method: Valid options are 'Classic', 'LSH' and 'Hypercube'");
    }
    


    if(!k){
        std::string n_of_clusters;
        cout << "Enter number of clusters: ";
        getline(cin, n_of_clusters);
        k = std::stoi(n_of_clusters);
    }


    ofstream output_file(out_path, ios::out);
    if (output_file.fail()) 
        throw runtime_error(out_path + " could not be opened!\n");

    printf("here1\n");
    DataSet dataset(input_path);

    uint32_t window = 4000;
    uint32_t table_size = dataset.size() / 8;
    printf("here2\n");
    Clusterer* clusterer = 
    parser.value<string>("m") == "Classic" ? 
        (Clusterer*)new Lloyd(dataset, k, l2_distance) :
        (Clusterer*)new RAssignment(dataset, k, 
                                    parser.value<string>("m") == "LSH" ? 
                                        (Approximator*)new LSH(dataset, window, lsh_k, L, table_size) : 
                                        (Approximator*)new Cube(dataset, window, cube_k, probes, M), 
                                    l2_distance, l2_distance);

    printf("here3\n");
    clusterer->apply();

    auto p = clusterer->get()[0]->points().begin();
    auto point = (*p)->data();
    for (size_t i = 0; i < point.len(); i++)
        printf("%3d%s", point[i], ((i + 1) % 28) == 0 ? "\n" : " ");
    printf("\n");
} 
catch (exception& e) {
    cerr << e.what();
    return -1;
}
    return 0;
}