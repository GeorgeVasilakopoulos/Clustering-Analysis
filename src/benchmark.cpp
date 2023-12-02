#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_set>
#include <cmath>
#include <string>
#include <filesystem>

#include "utils.hpp"
#include "HashTable.hpp"
#include "ArgParser.hpp"
#include "FileParser.hpp"
#include "lsh.hpp"
#include "cube.hpp"
#include "Graph.hpp"

#define _LSH  0
#define _CUBE 1
#define _GNNS 2
#define _MRNG 3

static const char* names[] = {"LSH ", "Cube", "GNNS", "MRNG"};

#define CALL_ASSERT(algo, call) 												\
	vec = call;																	\
	if (vec.size() == 0) {														\
		fprintf(stderr, "\n%s: No Nearest Neighbour found!\n", names[algo]);	\
		exit(EXIT_FAILURE);														\
	}																			\
	pair = vec[0];																\

#define METRICS(algo, call)											\
	swcout.start();													\
	CALL_ASSERT(algo, call)											\
	rtime[algo] += swcout.stop();									\
	acc[algo]	+= pair.first == actual_label;						\
	af[algo]	+= pair.second / actual_distance;					\
	maf[algo]	 = max(maf[algo], pair.second / actual_distance);	\


#define QUERIES 100

using namespace std;

int main(int argc, const char* argv[]){
try{

	ArgParser parser = ArgParser();

	parser.add("d", STRING);
	parser.add("q", STRING);
	parser.add("o", STRING);
	parser.add("c", STRING);
	parser.add("config", STRING);
	parser.add("size", UINT, "0");
	parser.add("gnns_load", STRING);
	parser.add("gnns_save", STRING);
	parser.add("mrng_load", STRING);
	parser.add("mrng_save", STRING);
	
	parser.parse(argc,argv);
	
    string save_path_gnns = parser.parsed("gnns_save") ? parser.value<string>("gnns_save") : "";
    string load_path_gnns = parser.parsed("gnns_load") ? parser.value<string>("gnns_load") : "";

    string save_path_mrng = parser.parsed("mrng_save") ? parser.value<string>("mrng_save") : "";
    string load_path_mrng = parser.parsed("mrng_load") ? parser.value<string>("mrng_load") : "";

	string data_path;
	string query_path;
	string out_path;
	string csv_path;
	string configuration_path;

	if(parser.parsed("d"))
		data_path = parser.value<string>("d");
	else
		throw runtime_error("Missing data path argument -d\n");
	
	if(parser.parsed("q"))
		query_path = parser.value<string>("q");
	else
		throw runtime_error("Missing data path argument -q\n");
	
	if(parser.parsed("o"))
		out_path = parser.value<string>("o");
	else
		throw runtime_error("Missing output path argument -o\n");
	
	if(parser.parsed("c"))
		csv_path = parser.value<string>("c");
	else
		throw runtime_error("Missing csv path argument -c\n");
	
	if(parser.parsed("config"))
		configuration_path = parser.value<string>("config");
	else
		throw runtime_error("Missing configuration path argument -config\n");
	


	ofstream output_file(out_path, ios::out);
	if (output_file.fail()) 
        throw runtime_error(out_path + " could not be opened!\n");
		
	ofstream csv_file(csv_path, ios::out | ios::app);
	if (output_file.fail()) 
        throw runtime_error(csv_path + " could not be opened!\n");

    Stopwatch swcout = Stopwatch();
	uint32_t size = parser.value<uint32_t>("size");
	cout << "Loading data... " << flush;
	swcout.start();
	DataSet train(data_path, size);
	cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)" << endl; 

	uint32_t window = 2600; // Average distance of points on given test dataset
	uint32_t table_size =  train.dim() / 8;


	FileParser file_parser = FileParser();
	file_parser.add("NearestNeighbors", "NearestNeighbors", 1);
	
	file_parser.add("lsh_k", "lsh_k", 4);
	file_parser.add("lsh_L", "lsh_L", 5);

	file_parser.add("cube_k", "cube_k", 14);
	file_parser.add("cube_M", "cube_M", 10);
	file_parser.add("cube_probes", "cube_probes", 2);
	
	file_parser.add("graph_approx", "graph_approx", 1);
	file_parser.add("graph_k", "graph_k", 50);
	file_parser.add("graph_E", "graph_E", 30);
	file_parser.add("graph_R", "graph_R", 1);
	file_parser.add("graph_l", "graph_l", 2);



	file_parser.parse(configuration_path);


	//////////////////////////////////
	//////////////LSH////////////////
	//////////////////////////////////


	uint32_t lsh_k = file_parser.value("lsh_k");
	uint32_t lsh_L = file_parser.value("lsh_L");


	cout << "Populating LSH HashTables... " << flush;
	swcout.start();
	LSH lsh(train, window, lsh_k, lsh_L, table_size);
	cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)" << endl; 


	//////////////////////////////////
	//////////////Cube////////////////
	//////////////////////////////////

	uint32_t cube_k 	 = file_parser.value("cube_k");
	uint32_t cube_probes = file_parser.value("cube_probes"); 
	uint32_t cube_M 	 = file_parser.value("cube_M");

	cout << "Populating Cube HashTable... " << flush;
	swcout.start();
	Cube cube(train, window, cube_k, cube_probes, cube_M);
	cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)" << endl;


	//////////////////////////////////
	//////////////GNN/////////////////
	//////////////////////////////////

	uint32_t approx_id = file_parser.value("graph_approx");
	uint32_t k = file_parser.value("graph_k");
	uint32_t R = file_parser.value("graph_R");
	uint32_t T = 20;
	uint32_t E = file_parser.value("graph_E");
	uint32_t l = file_parser.value("graph_l");
	
	cout << "Creating GNN graph... " << flush;
    swcout.start();
	GNNS gnns_graph = GNNS(train, approx_id == 1 ? (Approximator*)&lsh : (Approximator*)&cube,
						  l2_distance, k, R, T, E, load_path_gnns);
	cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)"<< endl; 

	if (!save_path_gnns.empty()) {
        cout << "Saving GNNS graph... " << flush;
        swcout.start();
        gnns_graph.save(save_path_gnns);
        cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)" << endl; 
    }

	//////////////////////////////////
	//////////////MRNG////////////////
	//////////////////////////////////

	cout << "Creating MRNG graph... " << flush;
    swcout.start();
	MRNG mrng_graph = MRNG(train, approx_id == 1 ? (Approximator*)&lsh : (Approximator*)&cube,
						l2_distance, l2_distance, k, l, load_path_mrng);
	cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)"<< endl; 

	if (!save_path_mrng.empty()) {
        cout << "Saving MRNG graph... " << flush;
        swcout.start();
        mrng_graph.save(save_path_mrng);
        cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)" << endl; 
    }


	cout << "Loading data... " << flush;
	swcout.start();
	DataSet test(query_path, QUERIES);
	cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)" << endl; 

	Vector<double> acc(4), rtime(4), af(4), maf(4, 1.);
	double bf_avg_time = 0;

	Stopwatch timer;
	timer.start();
	cout << "Beginning queries... " << flush;
	output_file << fixed << setprecision(4);
	vector<pair<uint32_t, double>> vec;
	for(auto q : test){

		//Brute Force
		swcout.start();
		auto pair = lsh.kNN(*q, 1, l2_distance)[0];
		bf_avg_time += swcout.stop();

		uint32_t actual_label = pair.first;
		double actual_distance = pair.second;
		
		METRICS(_LSH, lsh.kANN(*q, 1, l2_distance))
		METRICS(_CUBE, cube.kANN(*q, 1, l2_distance))
		METRICS(_GNNS, gnns_graph.query(q->data(), 1))
		METRICS(_MRNG, mrng_graph.query(q->data(), 1))
	}

	acc   /= test.size();
	af    /= test.size();
	rtime /= bf_avg_time;
	
	
	cout << "Done! (" << fixed << setprecision(3) << timer.stop() << " seconds)" << endl << endl; 
	
	output_file << "     | Accuracy | Approximation Factor |    MAF   | Relative Time Performance" << endl;
	output_file << "     |----------+----------------------+----------+--------------------------" << endl;
	output_file << fixed << setprecision(4);
	
	output_file << " LSH |  " 	 << acc[_LSH] << "  |        " 	 << af[_LSH] 
				<< "        |  " << maf[_LSH] << "  |          " << rtime[_LSH] << endl;
	
	output_file << "Cube |  " 	 << acc[_CUBE] << "  |        "   << af[_CUBE] 
				<< "        |  " << maf[_CUBE] << "  |          " << rtime[_CUBE] << endl;
	
	output_file << "GNNS |  " 	 << acc[_GNNS] << "  |        "   << af[_GNNS] 
				<< "        |  " << maf[_GNNS] << "  |          " << rtime[_GNNS] << endl;
	
	output_file << "MRNG |  " 	 << acc[_MRNG] << "  |        "   << af[_MRNG] 
				<< "        |  " << maf[_MRNG] << "  |          " << rtime[_MRNG] << endl;
	
	csv_file << "Accuracy," <<   acc[0] << "," <<   acc[1] << "," <<   acc[2] << "," <<   acc[3] << endl;
	csv_file << "AF,"		<<    af[0] << "," <<    af[1] << "," <<    af[2] << "," <<    af[3] << endl;
	csv_file << "MAF,"		<<   maf[0] << "," <<   maf[1] << "," <<   maf[2] << "," <<   maf[3] << endl;
	csv_file << "RTime,"	<< rtime[0] << "," << rtime[1] << "," << rtime[2] << "," << rtime[3] << endl;

}
catch (exception& e){
	cerr << e.what();
	return -1;
}
	return 0;
}


