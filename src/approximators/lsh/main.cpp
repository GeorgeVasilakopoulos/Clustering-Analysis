#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_set>
#include <cmath>

#include "utils.hpp"
#include "HashTable.hpp"
#include "ArgParser.hpp"
#include "lsh.hpp"

using namespace std;


int main(int argc, const char* argv[]) {
try {
	ArgParser parser = ArgParser();

	parser.add("d", STRING);
	parser.add("q", STRING);
	parser.add("o", STRING);
	parser.add("k", UINT, "4");
	parser.add("L", UINT, "5");
	parser.add("N", UINT, "1");
	parser.add("R", FLOAT, "10000.");

	parser.parse(argc, argv);

	string data_path;
	string query_path;
	string out_path;

	uint32_t k = parser.value<uint32_t>("k");
	uint32_t L = parser.value<uint32_t>("L");
	uint32_t N = parser.value<uint32_t>("N");
	float R	   = parser.value<float>("R");

	if (parser.parsed("d"))
		data_path = parser.value<string>("d");
	else {
		cout << "Enter path to dataset: ";
		getline(cin, data_path);
	}

	DataSet train(data_path);

	if (parser.parsed("q"))
		query_path = parser.value<string>("q");
	else {
		cout << "Enter path to query file: ";
		getline(cin, query_path);
	}

	if (parser.parsed("o"))
		out_path = parser.value<string>("o");
	else {
		cout << "Enter path to out file: ";
		getline(cin, out_path);
	}

	ofstream output_file(out_path, ios::out);
	if (output_file.fail()) 
        throw runtime_error(out_path + " could not be opened!\n");

	uint32_t window = 5;
	uint32_t table_size =  train.dim() / 8;
	// uint32_t window = 100;
	// uint32_t table_size =  100;
	LSH lsh(train, window, k, L, table_size);

	Stopwatch sw = Stopwatch();

	while (true) {
		for (auto point : DataSet(query_path, 10)) {

			sw.start();
			auto aknn = lsh.kANN(*point, N, l2_distance<uint8_t>);
			double lfs_time = sw.stop();
			auto range = lsh.RangeSearch(*point, R, l2_distance<uint8_t>);

			sw.start();
			auto knn = lsh.kNN(*point, N, l2_distance<uint8_t>);
			double true_time = sw.stop();

			
			output_file << "Query " << point->label() << "\n";

			for (uint32_t i = 0, size = aknn.size(); i < size; i++) {
				output_file << "Nearest neighbor-" << i << ": " << aknn[i].first << "\n";
				output_file << "distanceLSH: "  << aknn[i].second << "\n";
				output_file << "distanceTrue: " << knn[i].second   << "\n";
			}

			output_file << "tLSH: "  << lfs_time  << "\n";
			output_file << "tTrue: " << true_time << "\n\n";
			output_file << R << "-near neighbors:\n";

			for (auto vec : range)
				output_file << vec.first << "\n";
			output_file << "\n";
		}

		cout << "Enter path to new query file (Nothing in order to stop): ";
		getline(cin, query_path);

		if (query_path.empty()) 
			break;
	}
	
	output_file.close();
} 
catch (exception& e) {
	cerr << e.what();
	return -1;
}
	return 0;
}