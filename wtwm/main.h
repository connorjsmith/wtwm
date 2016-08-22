#pragma once
#include <unordered_map>
#include <string>
std::unordered_map< std::string, std::string > argv_to_args(int argc, char* argv[]);
void listen(std::unordered_map< std::string, std::string >& args);
