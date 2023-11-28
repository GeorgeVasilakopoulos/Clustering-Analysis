#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_set>
#include <cmath>
#include <string>

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

#define METRICS(algo) 												\
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
	parser.add("config", STRING);
	parser.add("size", UINT, "0");
	
	parser.parse(argc,argv);
	string data_path;
	string query_path;
	string out_path;
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
	
	if(parser.parsed("config"))
		configuration_path = parser.value<string>("config");
	else
		throw runtime_error("Missing configuration path argument -config\n");
	


	ofstream output_file(out_path, ios::out);
	if (output_file.fail()) 
        throw runtime_error(out_path + " could not be opened!\n");


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

	file_parser.add("cube_k", "cube_k", 7);
	file_parser.add("cube_M", "cube_M", 6000);
	file_parser.add("cube_probes", "cube_probes", 10);
	
	file_parser.add("graph_k", "graph_k", 3000);
	file_parser.add("graph_E", "graph_E", 30);
	file_parser.add("graph_R", "graph_R", 10);
	file_parser.add("graph_approx", "graph_approx", 1);

	file_parser.add("graph_l", "graph_l", 2000);


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
	uint32_t T = 10;
	uint32_t E = file_parser.value("graph_E");
	uint32_t l = file_parser.value("graph_l");
	
	cout << "Creating GNN graph... " << flush;
    swcout.start();
	GNNS gnn_graph = GNNS(train, approx_id == 1 ? (Approximator*)&lsh : (Approximator*)&cube,
						  l2_distance, k, R, T, E);
	cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)"<< endl; 


	//////////////////////////////////
	//////////////MRNG////////////////
	//////////////////////////////////

	cout << "Creating MRNG graph... " << flush;
    swcout.start();
	MRNG mrng_graph = MRNG(train, approx_id == 1 ? (Approximator*)&lsh : (Approximator*)&cube,
						l2_distance, l2_distance, k, l);
	cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)"<< endl; 



	cout << "Loading data... " << flush;
	swcout.start();
	DataSet test(query_path, QUERIES);
	cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)" << endl; 

	Vector<double> acc(4), rtime(4), af(4), maf(4, 1.);
	double bf_avg_time = 0;

	Stopwatch timer;
	timer.start();
	cout << "Beginning queries... " << flush;
	for(auto q : test){

		//Brute Force
		swcout.start();
		auto pair = lsh.kNN(*q, 1, l2_distance)[0];
		bf_avg_time += swcout.stop();

		uint32_t actual_label = pair.first;
		double actual_distance = pair.second;
		
		swcout.start();
		pair = lsh.kANN(*q, 1, l2_distance)[0];
		METRICS(_LSH)

		swcout.start();
		pair = cube.kANN(*q, 1, l2_distance)[0];
		METRICS(_CUBE)

		swcout.start();
		pair = gnn_graph.query(q->data(), 1)[0];
		METRICS(_GNNS)

		swcout.start();
		pair = mrng_graph.query(q->data(), 1)[0];
		METRICS(_MRNG)
	}

	rtime /= test.size();
	acc   /= test.size();
	af    /= test.size();
	
	
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

	// printf("     | Accuracy | Approximation Factor |    MAF   | Relative Time Performance\n");
	// printf("     |----------+----------------------+----------+--------------------------\n");
	// printf(" LSH |  %.4f  |        %.4f        |  %.4f  |          %.4f\n", acc[_LSH],  af[_LSH],  maf[_LSH],  rtime[_LSH]);
	// printf("Cube |  %.4f  |        %.4f        |  %.4f  |          %.4f\n", acc[_CUBE], af[_CUBE], maf[_CUBE], rtime[_CUBE]);
	// printf("GNSS |  %.4f  |        %.4f        |  %.4f  |          %.4f\n", acc[_GNNS], af[_GNNS], maf[_GNNS], rtime[_GNNS]);
	// printf("MRNG |  %.4f  |        %.4f        |  %.4f  |          %.4f\n", acc[_MRNG], af[_MRNG], maf[_MRNG], rtime[_MRNG]);



















}
catch (exception& e){
	cerr << e.what();
	return -1;
}
	return 0;
}


