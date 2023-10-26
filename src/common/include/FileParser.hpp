#pragma once

#include <string>
#include <unordered_map>
#include <iostream>


class FileParser {

    private:
        std::unordered_map<std::string, std::string> aliases;
        std::unordered_map<std::string, uint32_t> values;

    public:
        void add(std::string line, std::string alias, uint32_t def=0);
        bool parsed(std::string alias);
        uint32_t& value(std::string alias);

        void parse(std::string path);

        void print();
};

#include "../modules/FileParser.tcc"