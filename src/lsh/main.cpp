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

double dist(Vector<uint8_t>& v1, Vector<uint8_t>& v2){
	
	if (v1.len() != v2.len()) 
        throw std::runtime_error("Exception in Distance Metric: Dimensions of vectors must match!\n");

	double sum = 0;
	for(uint32_t i = 0; i < v1.len(); i++){
		double diff = (double)v1[i] - (double)v2[i];
		sum += diff * diff; 
	}

	return sqrt(sum);
}

std::vector<std::tuple<uint32_t, double>> 
kNN(DataSet& dataset, DataPoint& query, uint32_t k, double (*dist)(Vector<uint8_t>&, Vector<uint8_t>&)) {

	auto comparator = [](const std::tuple<uint32_t, double> t1, const std::tuple<uint32_t, double> t2) {
		return get<1>(t1) > get<1>(t2);
	};

	priority_queue<tuple<uint32_t, double>, vector<tuple<uint32_t, double>>, decltype(comparator)> knn(comparator);
	unordered_set<uint32_t> k_point_set;
	
	for(auto point : dataset) {
		if(query.label() == point->label())
			continue; 

		double distance = dist(query.data(), point->data());
		knn.push(std::make_tuple(point->label(), distance));
	}

	std::vector< tuple<uint32_t, double> > out;
	
	while(!knn.empty() && (int)k-- > 0) {
		out.push_back(knn.top());
		knn.pop();
	}

	return out;	
}

vector< tuple<uint32_t, double> > 
RangeSearch(DataSet& dataset, DataPoint& query, double range, double (*dist)(Vector<uint8_t>&, Vector<uint8_t>&)) {

	vector< tuple<uint32_t, double> > out;

	for(auto point : dataset) {

		if(query.label() == point->label())
			continue; 

		double distance = dist(query.data(), point->data());

		if(distance < range)
			out.push_back(make_tuple(point->label(),distance));
	}
	
	return out;
}


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

	std::string data_path;
	std::string query_path;
	std::string out_path;

	uint32_t k = parser.value<uint32_t>("k");
	uint32_t L = parser.value<uint32_t>("L");
	uint32_t N = parser.value<uint32_t>("N");
	float R	   = parser.value<float>("R");

	if (parser.parsed("d"))
		data_path = parser.value<std::string>("d");
	else {
		std::cout << "Enter path to dataset: ";
		std::getline(std::cin, data_path);
	}

	// time this
	DataSet train(data_path);

	if (parser.parsed("q"))
		query_path = parser.value<std::string>("q");
	else {
		std::cout << "Enter path to query file: ";
		std::getline(std::cin, query_path);
	}

	if (parser.parsed("o"))
		out_path = parser.value<std::string>("o");
	else {
		std::cout << "Enter path to out file: ";
		std::getline(std::cin, out_path);
	}

	std::ofstream output_file(out_path, std::ios::out);
	if (output_file.fail()) 
        throw std::runtime_error(out_path + " could not be opened!\n");

	uint32_t window = 5; // 100
	uint32_t table_size =  train.dim() / 8; // 100
	LSH lsh(train, window, k, L, table_size);

	Stopwatch sw = Stopwatch();

	while (true) {
		for (auto point : DataSet(query_path, 1)) {

			sw.start();
			auto aknn = lsh.kANN(*point, N, dist);
			double lfs_time = sw.stop();
			auto range = lsh.RangeSearch(*point, R, dist);

			sw.start();
			auto knn = kNN(train, *point, N, dist);
			double true_time = sw.stop();

			
			output_file << "Query " << point->label() << "\n";

			for (uint32_t i = 0, size = aknn.size(); i < size; i++) {
				output_file << "Nearest neighbor-" << i << ": " << std::get<0>(aknn[i]) << "\n";
				output_file << "distanceLSH: "  << std::get<1>(aknn[i]) << "\n";
				output_file << "distanceTrue: " << std::get<1>(knn[i])   << "\n";
			}

			output_file << "tLSH: "  << lfs_time  << "\n";
			output_file << "tTrue: " << true_time << "\n\n";
			output_file << R << "-near neighbors:\n";

			for (auto vec : range)
				output_file << std::get<0>(vec) << "\n";
			output_file << "\n";
		}
	
		std::cout << "Enter path to new query file (Nothing in order to stop): ";
		std::getline(std::cin, query_path);

		if (query_path.empty()) 
			break;	
	}


} 
catch (exception& e) {
	std::cerr << e.what();
	return -1;
}
	return 0;
}