#include <efs/efs.h>
#include <filesystem>
#include <fstream>
#include <unistd.h>

extern std::string working_folder; // from main.cpp

Buffer read_file(const char* path){
    std::ifstream file(path, std::ios::binary | std::ios::ate);

    Buffer result;
    result.ptr = nullptr;
    result.size = 0;

        if (!file){
        std::cout << "file read error!\n";
        return result;
    }

    result.size = static_cast<size_t>(file.tellg());
    result.ptr = malloc(result.size);

    file.seekg(0);
    file.read((char*)result.ptr, result.size);

    return result;
}

void write_file(const char* path, Buffer data){
    std::ofstream file(path, std::ios::binary);

    if (!file){
        std::cout << "file write error!\n";
        return;
    }

    file.write((char*)data.ptr, data.size);
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

std::string get_file_work_relative_path(std::string path){
    char cwd[4096];

    if (getcwd(cwd, sizeof(cwd)) == nullptr){
        return "";
    }

    std::filesystem::path work_dir(cwd);
    std::filesystem::path file_path(path);
    std::filesystem::path relative = std::filesystem::relative(file_path.parent_path(), work_dir);
    return relative.string();
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

file_time get_file_last_update(std::string path){
    auto ftime = std::filesystem::last_write_time(path);

    auto system_time = std::chrono::time_point_cast<std::chrono::system_clock::duration>(ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());

    return std::chrono::system_clock::to_time_t(system_time);
}
