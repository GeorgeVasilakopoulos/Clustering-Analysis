#include <iostream>
#include <cfloat>

#include "Graph.hpp"
#include "lsh.hpp"
#include "cube.hpp"
#include "ArgParser.hpp"

using namespace std;

#define QUERIES 10


int main(int argc, const char* argv[]) {
try {
    ArgParser parser = ArgParser();

    parser.add("d", STRING);
    parser.add("q", STRING);
    parser.add("o", STRING);
    parser.add("k", UINT, "50");
    parser.add("E", UINT, "30");
    parser.add("R", UINT, "1");
    parser.add("N", UINT, "1");
    parser.add("l", UINT, "20");
    parser.add("m", STRING);
    parser.add("a", STRING, "LSH");
    parser.parse(argc, argv);

    
	uint32_t k = parser.value<uint32_t>("k");
	uint32_t E = parser.value<uint32_t>("E");
	uint32_t R = parser.value<uint32_t>("R");
    uint32_t T = 10;
	uint32_t N = parser.value<uint32_t>("N");
	uint32_t l = parser.value<uint32_t>("l");
    
    string approx_method = parser.value<string>("a");

    string input_path;
    string query_path;
    string out_path;

    if(parser.parsed("d"))
        input_path = parser.value<string>("d");
    else {
        cout << "Enter path to input file: " << flush;
        getline(cin, input_path);
    }

    if(parser.parsed("q"))
        query_path = parser.value<string>("q");
    else {
        cout << "Enter path to query file: " << flush;
        getline(cin, query_path);
    }

    if(parser.parsed("o"))
        out_path = parser.value<string>("o");
    else {
        cout << "Enter path to output file: " << flush;
        getline(cin, out_path);
    }

	ofstream output_file(out_path, ios::out);
	if (output_file.fail()) 
        throw runtime_error(out_path + " could not be opened!\n");

    
    string graph_method;

    if(parser.parsed("m"))
        graph_method = parser.value<string>("m");
    else{
        cout << "Enter Graph method - 1 for GNNS, 2 for MRNG: " << flush;
        getline(cin, graph_method);
        if(graph_method != "1" && graph_method != "2")
            throw runtime_error("Invalid Graph Method: Valid options are '1' for GNNS and '2' for MRNG");
    }

    
    Stopwatch timer, timer_out;

    cout << "Loading input data... " << flush;
    timer.start();
    DataSet train_dataset(input_path);
    cout << "Done! (" << std::fixed << std::setprecision(3) << timer.stop() << " seconds)" << endl; 
    

    uint32_t lsh_k = 4, L = 5;
    uint32_t cube_k = 7, probes = 10, M = 6000;
    uint32_t window = 2600;
    uint32_t table_size = train_dataset.size() / 8;

    cout << "Initializing Approximators... " << flush;
    timer.start();
    LSH lsh   = LSH(train_dataset, window, lsh_k, L, table_size);
    Cube cube = Cube(train_dataset, window, cube_k, probes, M);
    cout << "Done! (" << std::fixed << std::setprecision(3) << timer.stop() << " seconds)"<< endl; 


    cout << "Creating graph... " << flush;
    timer.start();
    Graph* graph = 
    graph_method == "1" ? 
        (Graph*)new GNN(train_dataset, approx_method == "LSH" ? (Approximator*)&lsh : (Approximator*)&cube, 
                        l2_distance, k, R, T, E) :
        (Graph*)new MRNG(train_dataset, approx_method == "LSH" ? (Approximator*)&lsh : (Approximator*)&cube, 
                         l2_distance, l2_distance, k, l);
    cout << "Done! (" << std::fixed << std::setprecision(3) << timer.stop() << " seconds)"<< endl; 
    
    
	timer_out.start();
	cout << "Beginning search for \"" << query_path << "\"... " << flush;
	while (true) {
        double ttime_lsh = 0, ttime_cube = 0, ttime_graph = 0, ttime_true = 0;
        double tdist_lsh = 0, tdist_cube = 0, tdist_graph = 0, tdist_true = 0;
		for (auto point : DataSet(query_path, QUERIES)) {

			timer.start();
			auto aknn_graph = graph->query(point->data(), N);
			double graph_time = timer.stop();

			timer.start();
			auto aknn_lsh = lsh.kANN(*point, N, l2_distance<uint8_t>);
			double lsh_time = timer.stop();

			timer.start();
			auto aknn_cube = cube.kANN(*point, N, l2_distance<uint8_t>);
			double cube_time = timer.stop();

			timer.start();
			auto knn = lsh.kNN(*point, N, l2_distance<uint8_t>);
			double true_time = timer.stop();

			ttime_graph += graph_time;
			ttime_lsh   += lsh_time;
			ttime_cube  += cube_time;
			ttime_true  += true_time;
			
			output_file << "Query " << point->label() << "\n";

            double dist_graph = 0;
            double dist_true  = 0;
            double MAF        = DBL_MIN;
			for (uint32_t i = 0, size = aknn_graph.size(); i < size; i++) {
				output_file << "Nearest neighbor-" << i << ": " << aknn_graph[i].first << "\n";
				output_file << "distanceApproximate: "  << aknn_graph[i].second << "\n";
				output_file << "distanceTrue: " << knn[i].second   << "\n";
				dist_graph += aknn_graph[i].second;
				dist_true  += knn[i].second;
                double temp = dist_graph / dist_true;
                if (MAF < temp) {
                    MAF = temp;
                }
			}

            dist_graph /= aknn_graph.size();
            dist_true /= aknn_graph.size();
			output_file << "tAverageApproximate: "  << std::fixed << std::setprecision(4) << dist_graph << "\n";
			output_file << "tAverageTrue: " << std::fixed << std::setprecision(4) << dist_true << "\n";
			output_file << "MAF: " << std::fixed << std::setprecision(4) << MAF << "\n\n";

            const uint32_t size = std::min(aknn_graph.size(), std::min(aknn_lsh.size(), aknn_cube.size()));
            for (uint32_t i = 0; i < size; i++) {
				tdist_graph += aknn_graph[i].second;
				tdist_lsh   += aknn_lsh[i].second;
				tdist_cube  += aknn_cube[i].second;
				tdist_true  += knn[i].second;
            }
		}

		cout << "Done! (" << std::fixed << std::setprecision(3) << timer_out.stop() << " seconds)" << endl; 

		cout << 
        "Relative time performance (LSH/Cube/Graph time / True time): " << 
        std::fixed << std::setprecision(4) << ttime_lsh   / ttime_true << " / " << 
        std::fixed << std::setprecision(4) << ttime_cube  / ttime_true << " / " << 
        std::fixed << std::setprecision(4) << ttime_graph / ttime_true << endl; 

		cout << 
        "Approximation Factor (LSH/Cube/Graph dist / True dist): " << 
        std::fixed << std::setprecision(4) << tdist_lsh   / tdist_true << " / " << 
        std::fixed << std::setprecision(4) << tdist_cube  / tdist_true << " / " << 
        std::fixed << std::setprecision(4) << tdist_graph / tdist_true << endl; 

		cout << "\nEnter path to new query file (Nothing in order to stop): " << flush;
		getline(cin, query_path);

		if (query_path.empty()) 
			break;
	}
	
	output_file.close();
    delete graph;

} 
catch (exception& e) {
    cerr << e.what();
    return -1;
}
    return 0;
}