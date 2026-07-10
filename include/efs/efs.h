#ifndef efs_H
#define efs_H

#include <string>
#include <vector>
#include <uVRB_config.h>
#include <cstdint>

Buffer read_file(const char* path);

class filter{
public:
    string_list included_extensions;
    string_list excluded_directories;
};

string_list list_sub_files(const char* root_path);
string_list list_sub_files_with_filter(const char* root_path, filter filter_data);

std::string get_file_extension(std::string path);
std::string get_file_name(std::string path);
std::string get_file_relative_path(std::string path);

std::string pretty_string_list(string_list list);
std::string unpretty_string_list(string_list list);

#endif