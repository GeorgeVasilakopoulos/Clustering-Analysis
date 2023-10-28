#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_set>
#include <cmath>

#include "utils.hpp"
#include "HashTable.hpp"
#include "ArgParser.hpp"
#include "cube.hpp"

using namespace std;


int main(int argc, const char* argv[]) {
try {
	ArgParser* parser = new ArgParser();

	parser->add("d", 		STRING);
	parser->add("q", 		STRING);
	parser->add("o", 		STRING);
	parser->add("k", 		UINT, 	"14");
	parser->add("M", 		UINT, 	"10");
	parser->add("probes", 	UINT, 	"2");
	parser->add("N", 		UINT, 	"1");
	parser->add("R", 		FLOAT, 	"10000.");

	parser->parse(argc, argv);

	string data_path;
	string query_path;
	string out_path;

	uint32_t k      = parser->value<uint32_t>("k");
	uint32_t probes = parser->value<uint32_t>("probes");
	uint32_t points = parser->value<uint32_t>("M");
	uint32_t N      = parser->value<uint32_t>("N");
	float R	        = parser->value<float>("R");

	if (parser->parsed("d"))
		data_path = parser->value<string>("d");
	else {
		cout << "Enter path to dataset: " << flush;
		getline(cin, data_path);
	}

	Stopwatch swcout = Stopwatch();
	
	swcout.start();
	cout << "Loading data... " << flush;
	DataSet train(data_path);
	cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)" << endl; 

	if (parser->parsed("q"))
		query_path = parser->value<string>("q");
	else {
		cout << "Enter path to query file: " << flush;
		getline(cin, query_path);
	}

	if (parser->parsed("o"))
		out_path = parser->value<string>("o");
	else {
		cout << "Enter path to out file: " << flush;
		getline(cin, out_path);
	}

	ofstream output_file(out_path, ios::out);
	if (output_file.fail()) 
        throw runtime_error(out_path + " could not be opened!\n");

	uint32_t window = 2600;

	swcout.start();
	cout << "Populating HashTable... " << flush;
	Cube cube(train, window, k, probes, points);
	cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)" << endl;

	swcout.start();
	cout << "Beginning search for \"" << query_path << "\"... " << flush;

	Stopwatch sw = Stopwatch();
	while (true) {
		for (auto point : DataSet(query_path, 10)) {

			sw.start();
			auto aknn = cube.kANN(*point, N, l2_distance<uint8_t>);
			double lfs_time = sw.stop();
			auto range = cube.RangeSearch(*point, R, l2_distance<uint8_t>);

			sw.start();
			auto knn = cube.kNN(*point, N, l2_distance<uint8_t>);
			double true_time = sw.stop();

			
			output_file << "Query " << point->label() << "\n";

			for (uint32_t i = 0, size = aknn.size(); i < size; i++) {
				output_file << "Nearest neighbor-" << i + 1 << ": " << aknn[i].first << "\n";
				output_file << "distanceCube: "  << aknn[i].second << "\n";
				output_file << "distanceTrue: " << knn[i].second   << "\n";
			}

			output_file << "tCube: "  << lfs_time  << "\n";
			output_file << "tTrue: " << true_time << "\n\n";
			output_file << R << "-near neighbors:\n";

			for (auto vec : range)
				output_file << vec.first << "\n";
			output_file << "\n";
		}

		cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)" << endl; 
		cout << "Enter path to new query file (Nothing in order to stop): " << flush;
		getline(cin, query_path);

		if (query_path.empty()) 
			break;
	}
	
	output_file.close();
	delete parser;
}
catch (exception& e) {
	cerr << e.what();
	return -1;
}
	return 0;
}