#include <iostream>
#include <cfloat>

#include "Graph.hpp"
#include "lsh.hpp"
#include "cube.hpp"
#include "ArgParser.hpp"

using namespace std;

#define QUERIES 100


static string get_arg(ArgParser& parser, string flag, string msg) {
	string ret;

    if(parser.parsed(flag))
        ret = parser.value<string>(flag);
    else {
        cout << msg << flush;
        getline(cin, ret);
    }

	return ret;
}

int main(int argc, const char* argv[]) {
try {
    ArgParser parser = ArgParser();

    parser.add("d", STRING);
    parser.add("q", STRING);
    parser.add("dl", STRING);
    parser.add("ql", STRING);
    parser.add("o", STRING);
    parser.add("k", UINT, "50");
    parser.add("E", UINT, "30");
    parser.add("R", UINT, "1");
    parser.add("N", UINT, "1");
    parser.add("l", UINT, "20");
    parser.add("m", STRING);
    parser.add("a", STRING, "LSH");
    parser.add("save", STRING);
    parser.add("load", STRING);
    parser.parse(argc, argv);

    string save_path = parser.parsed("save") ? parser.value<string>("save") : "";
    string load_path = parser.parsed("load") ? parser.value<string>("load") : "";

    
	uint32_t k = parser.value<uint32_t>("k");
	uint32_t E = parser.value<uint32_t>("E");
	uint32_t R = parser.value<uint32_t>("R");
    uint32_t T = 10;
	uint32_t N = parser.value<uint32_t>("N");
	uint32_t l = parser.value<uint32_t>("l");
    
    string approx_method = parser.value<string>("a");

    string input_path, input_path_latent;
    string query_path, query_path_latent;
    string out_path;

    input_path = get_arg(parser, "d", "Enter path to input file: ");
    query_path = get_arg(parser, "q", "Enter path to query file: ");
    out_path   = get_arg(parser, "o", "Enter path to output file: ");

    input_path_latent = get_arg(parser, "dl", "Enter path to input file (Latent Space): ");
    query_path_latent = get_arg(parser, "ql", "Enter path to query file (Latent Space): ");

	ofstream output_file(out_path, ios::out);
	if (output_file.fail()) 
        throw runtime_error(out_path + " could not be opened!\n");

    string graph_method = get_arg(parser, "m", "Enter Graph method - 1 for GNNS, 2 for MRNG, 3 for BF: ");

    if(graph_method != "1" && graph_method != "2" && graph_method != "3")
        throw runtime_error("Invalid Graph Method: Valid options are '1' for GNNS, '2' for MRNG and '3' for BF");
    

    Stopwatch timer, timer_out;

    cout << "Loading input data... " << flush;
    timer.start();
    DataSet train_dataset(input_path);
    DataSet train_dataset_latent(input_path_latent);
    cout << "Done! (" << std::fixed << std::setprecision(3) << timer.stop() << " seconds)" << endl; 
    
    cout << "Initializing Approximators... " << flush;
    timer.start();
    Approximator approx = Approximator(train_dataset);
    Approximator approx_latent = Approximator(train_dataset_latent);
    cout << "Done! (" << std::fixed << std::setprecision(3) << timer.stop() << " seconds)"<< endl; 


    cout << "Creating graph... " << flush;
    timer.start();
    Graph* graph = 
    graph_method == "1" ? 
        (Graph*)new GNNS(train_dataset_latent, &approx_latent, l2_distance, k, R, T, E, load_path) :
    graph_method == "2" ?
        (Graph*)new MRNG(train_dataset_latent, &approx_latent, l2_distance, l2_distance, k, l, load_path) :
    NULL;
    cout << "Done! (" << std::fixed << std::setprecision(3) << timer.stop() << " seconds)" << endl; 
    
    if (!save_path.empty()) {
        cout << "Saving graph... " << flush;
        timer.start();
        graph->save(save_path);
        cout << "Done! (" << std::fixed << std::setprecision(3) << timer.stop() << " seconds)" << endl; 
    }
    
	timer_out.start();
	while (true) {
	cout << "Beginning search for \"" << query_path << "\"... " << flush;
        double ttime_graph = 0, ttime_true = 0;
        double total_AF = 0, total_MAF = 0;

        DataSet test(query_path, QUERIES);
        DataSet test_latent(query_path_latent, QUERIES);

		for (size_t i = 0; i < QUERIES; i++) {
            auto point = test[i];
            auto point_latent = test_latent[i];

			timer.start();
			auto aknn_graph = graph ? 
                                graph->query(point_latent->data(), N) : 
                                approx_latent.kNN(*point_latent, 10, l2_distance);
			double graph_time = timer.stop();

			timer.start();
			auto knn = approx.kNN(*point, N, l2_distance<uint8_t>);
			double knn_time = timer.stop();

			ttime_graph += graph_time;
			ttime_true += knn_time;
			
			output_file << "Query " << point->label() << "\n";

            double dist_graph = 0;
            double dist_true  = 0;
            double AF         = 0;
            double MAF        = DBL_MIN;
			for (uint32_t i = 0, size = aknn_graph.size(); i < size; i++) {
				output_file << "Nearest neighbor-" << i << ": " << aknn_graph[i].first << "\n";
				output_file << "distanceApproximate: "  << aknn_graph[i].second << "\n";
				output_file << "distanceTrue: " << knn[i].second   << "\n";

				dist_graph += aknn_graph[i].second;
				dist_true  += knn[i].second;

                double pdist = l2_distance<uint8_t, uint8_t>(point->data(), train_dataset[aknn_graph[i].first - 1]->data());
                double temp = pdist / knn[i].second;
                if (MAF < temp)
                    MAF = temp;
                
                AF += temp / size;
			}

            total_AF  += AF;
            total_MAF += MAF;

			output_file << "tAverageApproximate: "  
                        << std::fixed << std::setprecision(4) << ttime_graph / aknn_graph.size() << "\n";
			output_file << "tAverageTrue: " 
                        << std::fixed << std::setprecision(4) << ttime_true / aknn_graph.size() << "\n";
			output_file << "AF: " << std::fixed << std::setprecision(4) << AF << "\n";
			output_file << "MAF: " << std::fixed << std::setprecision(4) << MAF << "\n\n";
		}

		cout << "Done! (" << std::fixed << std::setprecision(3) << timer_out.stop() << " seconds)" << endl; 

		cout << 
        "Relative time performance (Graph time / True time): " << 
        std::fixed << std::setprecision(4) << ttime_graph / ttime_true << endl; 

		cout << 
        "Approximation Factor (Graph dist / True dist): " << 
        std::fixed << std::setprecision(4) << total_AF / QUERIES << endl; 

		cout << 
        "Maximum Approximation Factor (Graph dist / True dist): " << 
        std::fixed << std::setprecision(4) << total_MAF / QUERIES << endl; 

		cout << "\nEnter path to new query file (Nothing in order to stop): " << flush;
		getline(cin, query_path);

		if (query_path.empty()) 
			break;

		cout << "\nEnter path to new query file (Latent space) (Nothing in order to stop): " << flush;
		getline(cin, query_path_latent);

		if (query_path_latent.empty()) 
			break;
	}
	
	output_file.close();

    if (graph)
        delete graph;
} 
catch (exception& e) {
    cerr << e.what();
    return -1;
}
    return 0;
}