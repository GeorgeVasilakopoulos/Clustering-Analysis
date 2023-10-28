#include "ArgParser.hpp"

static void* parse_arg(Type type, const std::string arg) {
    return  type == STRING ? (void*)new std::string(arg) : 
            type == UINT   ? (void*)new uint32_t(std::stoul(arg, nullptr, 0)) : 
            type == FLOAT  ? (void*)new float(std::stof(arg)) : 
                             (void*)new bool(arg == "true");
}

static void dealloc(Type type, void* value) {
    if (type == STRING)     delete (std::string*)value;
    else if (type == UINT)  delete (uint32_t*)value;
    else if (type == FLOAT) delete (float*)value;
    else                    delete (bool*)value;
}

ArgParser::~ArgParser() {
    for (auto& [key, value] : flags) 
        dealloc(types[key], value);
}

void ArgParser::add(std::string flag, Type type, std::string def) {
    types[flag] = type;
    flags[flag] = !def.empty() ? parse_arg(type, def) : nullptr;
}

bool ArgParser::parsed(std::string flag) { return flags.find(flag) != flags.end() && flags[flag] != nullptr; }



template <typename T>
T& ArgParser::value(std::string flag) { 
    if (!parsed(flag))
        throw std::runtime_error("Exception in Argument Retrieval operation: Argument " + flag + " was not parsed!\n");
    
    return *(T*)flags[flag];
}


void ArgParser::parse(uint32_t argc, const char** argv) {
    for (uint32_t i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '\0') {            
            std::string name = std::string(argv[i]).erase(0, 1);
            
            if (parsed(name))
                dealloc(types[name], flags[name]);

            if (types[name] != BOOL && i + 1 >= argc)
                throw std::runtime_error("Exception in Argument Parsing operation: Non boolean Argument " 
                                         + name + " is not followed by a value!\n");

            flags[name] = parse_arg(types[name], types[name] == BOOL ? "true" : std::string(argv[i++ + 1]));
        }
    }
}