#include <fstream>
#include <sstream>


void FileParser::parse(std::string path) {
    std::ifstream input(path.data(), std::ifstream::in);

	 if (input.fail()) 
        throw std::runtime_error("Failed to open file\n");
    

	std::string line;
	while (std::getline(input, line)) {
		if (line.empty())
			continue;

		std::istringstream lineStream(line);
		std::string label, number;
		
		if(!(std::getline(lineStream, label, ':') && lineStream >> number))
			throw std::runtime_error("Exception in Input file Parsing: Bad Format\n");
		
		if (values.find(label) == values.end())
			continue;

		values[label] = std::stoul(number);
	}

	input.close();
}

bool FileParser::parsed(std::string alias) { 
	return aliases.find(alias) != aliases.end() && values.find(aliases[alias]) != values.end() && values[aliases[alias]] != 0;
}

uint32_t& FileParser::value(std::string alias) {
	if (!parsed(alias))
        throw std::runtime_error("Exception in Parameter Retrieval operation: Parameter with alias " + alias + " was not parsed!\n");
    
    return values[aliases[alias]];
}

void FileParser::add(std::string line, std::string alias, uint32_t def) {
	aliases[alias] = line;
	values[line] = def;
}

void FileParser::print() {
	for (auto const& [key, val] : values) 
		std::cout << key << ':' << val << std::endl;

}



