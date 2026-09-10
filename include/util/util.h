#ifndef util_H
#define util_H

#include <uVRB_config.h>
#include <cstdint>
#include <fstream>

struct command_output{
    int return_code = -1;
    string_list out;
};

string_list parse_with(std::string input, char seperator);

string_list xparse_with(std::string input, char seperator, char bracket = '\"');

std::string remove_chars(std::string input, std::string chrs);

struct DependencyFile
{
    std::string target;
    string_list dependencies;
};

DependencyFile parse_dependency_file(const std::string& path);

command_output run_command(const char* command);

#endif