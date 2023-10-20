#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_set>
#include <cmath>

#include "utils.hpp"
#include "HashTable.hpp"
#include "ArgParser.hpp"
#include "FileParser.hpp"
#include "lsh.hpp"

using namespace std;


int main(int argc, const char* argv[]) {
try {
    ArgParser parser = ArgParser();

    parser.add("i", STRING);
    parser.add("c", STRING);
    parser.add("o", STRING);
    parser.add("complete", STRING);
    parser.add("m", UINT, "5");
    parser.parse(argc, argv);

    string input_path;
    string configuration_path;
    string out_path;
    string approx_method;

    uint32_t number_of_clusters=0, number_of_vector_hash_tables = 3;
    uint32_t number_of_vector_hash_functions = 4, max_number_M_hypercube = 10;
    uint32_t number_of_hypercube_dimensions = 2, number_of_probes = 2;


    if(parser.parsed("i"))
        input_path = parser.value<string>("i");
    else{
        cout << "Enter path to input file: ";
        getline(cin,input_path);
    }


    if(parser.parsed("c"))
        configuration_path = parser.value<string>("c");
    else{
        cout << "Enter path to configuration file: ";
        getline(cin,configuration_path);
    }

    if(parser.parsed("o"))
        out_path = parser.value<string>("o");
    else{
        cout << "Enter path to output file: ";
        getline(cin,out_path);
    }

    if(parser.parsed("m"))
        approx_method = parser.value<string>("m");
    else{
        cout << "Enter approximator method: ";
        getline(cin,approx_method);
        if(approx_method != "Classic" && approx_method != "LSH" && approx_method != "Hypercube")
            throw runtime_error("Invalid Approximator Method: Valid options are 'Classic', 'LSH' and 'Hypercube'");
    }
    
    FileParser fparser;
    fparser.parse(configuration_path);

    
    
    #define RESOLVE_VARIABLE(X)     \
        if(fparser.parsed(#X)){     \
            X = fparser.value(#X);  \
        }                           \

    RESOLVE_VARIABLE(number_of_clusters);
    RESOLVE_VARIABLE(number_of_vector_hash_tables);
    RESOLVE_VARIABLE(number_of_vector_hash_functions);
    RESOLVE_VARIABLE(max_number_M_hypercube);
    RESOLVE_VARIABLE(number_of_hypercube_dimensions);
    RESOLVE_VARIABLE(number_of_probes);


    if(!number_of_clusters){
        std::string n_of_clusters;
        cout << "Enter number of clusters: ";
        getline(cin,n_of_clusters);
        number_of_clusters = std::stoi(n_of_clusters);
    }




    ofstream output_file(out_path, ios::out);
    if (output_file.fail()) 
        throw runtime_error(out_path + " could not be opened!\n");

    DataSet dataset(input_path);



    //To be continued
} 
catch (exception& e) {
    cerr << e.what();
    return -1;
}
    return 0;
}