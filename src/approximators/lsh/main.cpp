#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_set>
#include <cmath>
#include <omp.h>

#include "utils.hpp"
#include "HashTable.hpp"
#include "ArgParser.hpp"
#include "lsh.hpp"

using namespace std;


static double avg_dist(const DataSet& data) {
	double sum = 0.0;

	#pragma omp parallel for num_threads(8) reduction(+:sum)
	for (size_t i = 0; i < data.size(); i++) {
		printf("i: %ld\n", i);
		for (size_t j = i + 1; j < data.size(); j++) {
			sum += l2_distance<uint8_t, uint8_t>(data[i]->data(), data[j]->data());
		}
	}

	return 2 * sum / (data.size() - 1) / data.size();
}

static double get_dist(DataPoint* p1, DataPoint* p2) {
	return l2_distance<uint8_t, uint8_t>(p1->data(), p2->data());
}

#define QUERIES 100

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
		cout << "Enter path to dataset: " << flush;
		getline(cin, data_path);
	}

	Stopwatch swcout = Stopwatch();

	cout << "Loading data... " << flush;
	swcout.start();
	DataSet train(data_path);
	DataSet train_latent("/mnt/c/Users/10geo/Documents/GitHub/Project/input/latent_train_images");
	cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)" << endl; 

	if (parser.parsed("q"))
		query_path = parser.value<string>("q");
	else {
		cout << "Enter path to query file: " << flush;
		getline(cin, query_path);
	}

	if (parser.parsed("o"))
		out_path = parser.value<string>("o");
	else {
		cout << "Enter path to out file: " << flush;
		getline(cin, out_path);
	}

	ofstream output_file(out_path, ios::out);
	if (output_file.fail()) 
        throw runtime_error(out_path + " could not be opened!\n");

	// uint32_t window = 2600; // Average distance of points on given test dataset
	// uint32_t table_size =  train.size() / 8;
	// uint32_t window = 105; // Average distance of points on given test dataset
	uint32_t window = 412; // Average distance of points on given test dataset
	// uint32_t table_size =  train_latent.dim() / 8;
	uint32_t table_size =  train_latent.size() / 8;

	// printf("table size: %d %d\n", table_size, train_latent.size() / 8);
	// printf("avg dist: %lf\n", avg_dist(train_latent));
	// exit(0);

	swcout.start();
	cout << "Populating HashTables... " << flush;
	LSH lsh(train, 10, 1, 1, 1);
	LSH lsh_latent(train_latent, window, k, L, table_size);
	cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)" << endl; 


	swcout.start();
	cout << "Beginning search for \"" << query_path << "\"... " << flush;

	Stopwatch sw = Stopwatch();

	DataSet test(query_path, QUERIES);
	DataSet test_latent("/mnt/c/Users/10geo/Documents/GitHub/Project/input/latent_test_images", QUERIES);

	while (true) {
		double ttime_lsh = 0, ttime_true = 0;
		double tdist_lsh = 0, tdist_true = 0;
		for (size_t i = 0; i < QUERIES; i++) {
			auto point = test[i];
			auto point_latent = test_latent[i];

			sw.start();
			// auto aknn = lsh_latent.kANN(*point_latent, N, l2_distance<uint8_t>);
			auto aknn = lsh_latent.kNN(*point_latent, N, l2_distance<uint8_t>);
			double lsh_time = sw.stop();
			auto range = lsh_latent.RangeSearch(*point_latent, R, l2_distance<uint8_t>);

			sw.start();
			auto knn = lsh.kNN(*point, N, l2_distance<uint8_t>);
			double true_time = sw.stop();

			ttime_lsh += lsh_time;
			ttime_true += true_time;
			
			output_file << "Query " << point->label() << "\n";

			for (uint32_t j = 0, size = aknn.size(); j < size; j++) {
				double dist = get_dist(point, train[aknn[j].first - 1]);
				output_file << "-Nearest neighbor of- " << i << "\n";
				output_file << "Nearest neighbor-" << j << ": " << aknn[j].first << "\n";
				output_file << "Nearest neighbor-" << j << ": " << knn[j].first << "\n";
				// output_file << "distanceLSH: "  << aknn[j].second << "\n";
				output_file << "distanceLSHLatent: "  << aknn[j].second << "\n";
				output_file << "distanceLSH      : "  << dist << "\n";
				output_file << "distanceTrue     : " << knn[j].second   << "\n";

				// tdist_lsh  += aknn[j].second;
				tdist_lsh  += dist;
				tdist_true += knn[j].second;
			}

			output_file << "tLSH: "  << lsh_time  << "\n";
			output_file << "tTrue: " << true_time << "\n\n";
			output_file << R << "-near neighbors:\n";

			for (auto vec : range)
				output_file << vec.first << "\n";
			output_file << "\n";
		}

		cout << "Done! (" << std::fixed << std::setprecision(3) << swcout.stop() << " seconds)" << endl; 
		cout << "Relative time performance (LSH time / True time): " << std::fixed << std::setprecision(3) << ttime_lsh / ttime_true << endl; 
		cout << "Approximation Factor (LSH dist / True dist): " << std::fixed << std::setprecision(3) << tdist_lsh / tdist_true << endl; 
		cout << "\nEnter path to new query file (Nothing in order to stop): " << flush;
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