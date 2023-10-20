#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>


class FileParser {

    private:
        std::unordered_map<std::string, uint32_t> labels;

    public:
        // ~FileParser();
        bool parsed(std::string label);
        uint32_t value(std::string label);
        void parse(std::string path);
};
