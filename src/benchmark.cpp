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


#define QUERIES 5000

using namespace std;

int main(int argc, const char* argv[]){
try{

	ArgParser parser = ArgParser();

	parser.add("d", STRING);
	parser.add("q", STRING);
	parser.add("o", STRING);
	parser.add("config",STRING);
	
	parser.parse(argc,argv);
	string data_path;
	string query_path;
	string out_path;
	string configuration_path;

	if(parser.parsed("d")){
		data_path = parser.value<string>("d");
	}
	else{
		throw runtime_error("Missing data path argument -d\n");
	}

	
	if(parser.parsed("q")){
		query_path = parser.value<string>("q");
	}
	else{
		throw runtime_error("Missing data path argument -q\n");
	}

	if(parser.parsed("o")){
		out_path = parser.value<string>("o");
	}
	else{
		throw runtime_error("Missing output path argument -o\n");
	}

	
	if(parser.parsed("config")){
		configuration_path = parser.value<string>("config");
	}
	else{
		throw runtime_error("Missing configuration path argument -config\n");
	}


	ofstream output_file(out_path, ios::out);
	if (output_file.fail()) 
        throw runtime_error(out_path + " could not be opened!\n");


    Stopwatch swcout = Stopwatch();

	cout << "Loading data... " << flush;
	swcout.start();
	DataSet train(data_path);
	cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)" << endl; 

	uint32_t window = 2600; // Average distance of points on given test dataset
	uint32_t table_size =  train.dim() / 8;




	FileParser file_parser = FileParser();
	file_parser.add("NearestNeighbors", "NearestNeighbors", 1);
	
	file_parser.add("lsh_k", "lsh_k",4);
	file_parser.add("lsh_L", "lsh_L",5);

	file_parser.add("cube_k", "cube_k",14);
	file_parser.add("cube_M", "cube_M",10);
	file_parser.add("cube_probes", "cube_probes",2);
	
	file_parser.add("graph_k", "graph_k",50);
	file_parser.add("graph_E", "graph_E",30);
	file_parser.add("graph_R", "graph_R",1);
	file_parser.add("graph_approx", "graph_approx",1);

	file_parser.add("graph_l","graph_l",20);



	file_parser.parse(configuration_path);


	//////////////////////////////////
	//////////////LSH////////////////
	//////////////////////////////////


	uint32_t k = file_parser.value("lsh_k");
	uint32_t L = file_parser.value("lsh_L");


	cout << "Populating LSH HashTables... " << flush;
	swcout.start();
	LSH lsh(train, window, k, L, table_size);
	cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)" << endl; 


	//////////////////////////////////
	//////////////Cube////////////////
	//////////////////////////////////

	k = file_parser.value("cube_k");
	uint32_t points = file_parser.value("cube_M");
	uint32_t probes = file_parser.value("cube_probes"); 

	cout << "Populating Cube HashTable... " << flush;
	swcout.start();
	Cube cube(train, window, k, probes, points);
	cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)" << endl;


	//////////////////////////////////
	//////////////GNN/////////////////
	//////////////////////////////////

	k = file_parser.value("graph_k");
	uint32_t E = file_parser.value("graph_E");
	uint32_t R = file_parser.value("graph_R");
	uint32_t approx_id = file_parser.value("graph_approx");
	uint32_t T = 10;
	uint32_t l = file_parser.value("graph_l");
	
	cout << "Creating GNN graph... " << flush;
    swcout.start();
	GNN gnn_graph = GNN(train,approx_id == 1 ? (Approximator*)&lsh : (Approximator*)&cube,
						l2_distance, k, R, T, E);
	cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)"<< endl; 


	//////////////////////////////////
	//////////////MRNG////////////////
	//////////////////////////////////

	cout << "Creating MRNG graph... " << flush;
    swcout.start();
	MRNG mrng_graph = MRNG(train,approx_id == 1 ? (Approximator*)&lsh : (Approximator*)&cube,
						l2_distance, l2_distance, k, l);
	cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)"<< endl; 



	cout << "Loading data... " << flush;
	swcout.start();
	DataSet test(query_path, QUERIES);
	cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)" << endl; 

	double lsh_avg_time = 0; double lsh_accuracy = 0; uint32_t lsh_avg_factor = 0; double lsh_max_factor = 1; 
	double cube_avg_time = 0; double cube_accuracy = 0; uint32_t cube_avg_factor = 0; double cube_max_factor = 1; 
	double mrng_avg_time = 0; double mrng_accuracy = 0; uint32_t mrng_avg_factor = 0; double mrng_max_factor = 1; 
	double gnn_avg_time = 0; double gnn_accuracy = 0; uint32_t gnn_avg_factor = 0; double gnn_max_factor = 1; 
	double bf_avg_time = 0;

	for(auto q : test){

		//Brute Force
		swcout.start();
		auto pair = lsh.kNN(*q,1,l2_distance)[0];

		bf_avg_time += swcout.stop();
		uint32_t actual_label = pair.first;
		double actual_distance = pair.second;
		

		//LSH
		swcout.start();
		pair = lsh.kANN(*q,1,l2_distance)[0];
		
		lsh_avg_time += swcout.stop();
		uint32_t lsh_label = pair.first;
		double distance = pair.second;
		lsh_accuracy += (lsh_label == actual_label);
		lsh_avg_factor += (distance/actual_distance);
		lsh_max_factor = max(lsh_max_factor, distance/actual_distance);


		//Cube
		swcout.start();
		pair = cube.kANN(*q,1,l2_distance)[0];
		
		cube_avg_time += swcout.stop();
		uint32_t cube_label = pair.first;
		distance = pair.second;
		cube_accuracy += (cube_label == actual_label);
		cube_avg_factor += (distance/actual_distance);
		cube_max_factor = max(cube_max_factor, distance/actual_distance);

		//GNN
		swcout.start();
		pair = gnn_graph.query(q->data(),1)[0];
		
		gnn_avg_time += swcout.stop();
		uint32_t gnn_label = pair.first;
		distance = pair.second;
		gnn_accuracy += (gnn_label == actual_label);
		gnn_avg_factor += (distance/actual_distance);
		gnn_max_factor = max(gnn_max_factor, distance/actual_distance);

		//MRNG
		swcout.start();
		pair = mrng_graph.query(q->data(),1)[0];
		
		mrng_avg_time += swcout.stop();
		uint32_t mrng_label = pair.first;
		distance = pair.second;
		mrng_accuracy += (mrng_label == actual_label);
		mrng_avg_factor += (distance/actual_distance);
		mrng_max_factor = max(mrng_max_factor, distance/actual_distance);		
	}


	lsh_avg_factor /= test.size();
	lsh_avg_time /= test.size();
	lsh_accuracy /= test.size();

	cube_avg_factor /= test.size();
	cube_avg_time /= test.size();
	cube_accuracy /= test.size();

	mrng_avg_factor /= test.size();
	mrng_avg_time /= test.size();
	mrng_accuracy /= test.size();

	gnn_avg_factor /= test.size();
	gnn_avg_time /= test.size();
	gnn_accuracy /= test.size();



	std::cout << std::fixed << std::setprecision(5);
	cout << "##### Accuracy # Approx Factor # Max Approx Factor # Avg Time" << endl;
	cout << "LSH:  " << lsh_accuracy << " " << lsh_avg_factor << " " << lsh_max_factor << " " << lsh_avg_time << endl;
	cout << "Cube: " << cube_accuracy << " " << cube_avg_factor << " " << cube_max_factor << " " << cube_avg_time << endl;
	cout << "GNN:  " << gnn_accuracy << " " << gnn_avg_factor << " " << gnn_max_factor << " " << gnn_avg_time << endl;
	cout << "MRNG: " << mrng_accuracy << " " << mrng_avg_factor << " " << mrng_max_factor << " " << mrng_avg_time << endl;
		






















}
catch (exception& e){
	cerr << e.what();
	return -1;
}
	return 0;
}


