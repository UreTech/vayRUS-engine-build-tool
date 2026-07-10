#include <efs/efs.h>
#include <filesystem>
#include <fstream>

extern std::string working_folder; // from main.cpp

Buffer read_file(const char* path){
    std::string full_path = working_folder + "/" + path;
    std::ifstream file(full_path, std::ios::binary | std::ios::ate);

    Buffer result;
    result.ptr = nullptr;
    result.size = 0;

    if (!file) return result;

    result.size = static_cast<size_t>(file.tellg());
    result.ptr = malloc(result.size);

    file.seekg(0);
    file.read((char*)result.ptr, result.size);

    return result;
}

// helpers
std::string get_file_extension(std::string path){
    for(int i = path.size() - 1; i >= 0; i--){
        if(path[i] == '.'){
            return path.substr(i);
        }else if(path[i] == '/'){
            return "";
        }
    }

    return "";
}

std::string get_file_name(std::string path){
    if (path.empty()) return "";

    size_t end = path.size();
    for(int i = path.size() - 1; i >= 0; i--){
        if(path[i] == '/'){
            return path.substr(i + 1, end - (i + 1));
        }else if(path[i] == '.'){
            end = i;
        }
    }

    return path.substr(0, end);
}

std::string get_file_relative_path(std::string path){
    for(int i = path.size() - 1; i >= 0; i--){
        if(path[i] == '/'){
            return path.substr(0, i + 1);
        }
    }

    return path;
}

bool check_filters(std::string path, filter filter_data){
    bool fart = true;

    std::string ext = get_file_extension(path);

    for(int i = 0; i < filter_data.included_extensions.size(); i++){
        if(ext == filter_data.included_extensions[i]){
            fart = false;
            break;
        }
    }

    if(fart){
        return false;
    }

    std::string r_path = get_file_relative_path(path);

    for(int i = 0; i < filter_data.excluded_directories.size(); i++){
        if(r_path == filter_data.excluded_directories[i].substr(0, r_path.size())){
            fart = true;
            break;
        }
    }

    if(fart){
        return false;
    }

    return true;
}

string_list list_sub_files_with_filter(const char* root_path, filter filter_data){
    std::string full_path = working_folder + "/" + root_path;

    string_list result;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(full_path))
    {
        if (entry.is_regular_file())
        {
            std::string path = entry.path().generic_string();
            if(check_filters(path, filter_data)){
                result.push_back(path);
            }
        }
    }

    return result;
}

string_list list_sub_files(const char* root_path){
    std::string full_path = working_folder + "/" + root_path;

    string_list result;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(full_path))
    {
        if (entry.is_regular_file())
        {
            result.push_back(entry.path().generic_string());
        }
    }

    return result;
}

std::string pretty_string_list(string_list list){
    std::string result;
    for(int i = 0; i < list.size(); i++){
        result += list[i] + "\n";
    }
    return result;
}

std::string unpretty_string_list(string_list list){
    std::string result;
    for(int i = 0; i < list.size(); i++){
        result += "\"" + list[i] + "\" ";
    }
    if(!result.empty()){
        result.pop_back();
    }
    return result;
}