#pragma once

#include <string>
#include <unordered_map>
#include <iostream>

typedef enum { STRING, UINT, FLOAT, BOOL } Type;

class ArgParser {

    private:
        std::unordered_map<std::string, void*> flags;
        std::unordered_map<std::string, Type> types;

    public:
        ~ArgParser();

        void add(std::string flag, Type type, std::string def="");
        bool parsed(std::string flag);
        template <typename T>
        T& value(std::string flag) { 
            if (!parsed(flag))
                throw std::runtime_error("Exception in Argument Retrieval operation: Argument " + flag + " was not parsed!\n");
            
            return *(T*)flags[flag];
        }

        void parse(uint32_t argc, const char** argv);
};