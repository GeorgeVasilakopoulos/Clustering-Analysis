#include "FileParser.hpp"



void FileParser::parse(std::string path){

	std::ifstream inputFile(path);
	if(!inputFile.is_open())
		throw std::runtime_error("Exception in Input file Parsing: Unable to open file " + path +"\n");


	std::string line;
	while(std::getline(inputFile,line)){
		std::istringstream lineStream(line);
		std::string label, number;
		if(std::getline(lineStream,label,':') && lineStream >> number){
			// if(parsed(label));
			// 	delete labels[label];
			uint32_t value = std::stoi(number);
			labels[label] = value;

		}
		else
			throw std::runtime_error("Exception in Input file Parsing: Bad Format\n");
	}
	inputFile.close();
}

bool FileParser::parsed(std::string label) { return labels.find(label) != labels.end(); }

uint32_t FileParser::value(std::string label){
	if(!parsed(label))
		throw std::runtime_error("Exception in Input file Parsing: Argument" + label + "was not parsed!\n");
	return labels[label];
}


// FileParser::~FileParser(){
// 	for(auto label : labels){
// 		delete label;
// 	}
// }

