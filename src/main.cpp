#include <iostream>
#include <cstdint>

#include <uVRB_config.h>

#include <uVar/uVar.h>

#include <efs/efs.h>
#include <filesystem>
#include <fstream>

#include <util/util.h>

struct filter_var{
    std::string var_name = "";
    filter flt;
};

struct list_var{
    std::string var_name = "";
    string_list list;
};

struct build_cache_line_t0{
    std::string in_file_path = "";
    std::string out_file_path = "";

    DependencyFile d; // filled by KEEP changed command
};

typedef std::vector<build_cache_line_t0> build_cache_t0;

build_cache_t0 read_build_cache_t0(std::string path){
    build_cache_t0 result;

    Buffer raw_cache = read_file(path.c_str());
    if(raw_cache.ptr == nullptr){
        std::cout << "Cache file \"" << path << "\" is not found or corrupted\n";
        UNKNOWN_CACHE_ERROR();
        return result;
    }

    std::string cache_str = std::string((char*)raw_cache.ptr, (char*)raw_cache.ptr + raw_cache.size);

    string_list lines = parse_with(cache_str, '\n');

    for(size_t i = 0; i < lines.size(); i++){
        std::string line = lines[i];
        
        line = remove_chars(line, "\n\r");
        
        if(line.empty()){
            continue; // skip
        }

        string_list parsed_line = xparse_with(line, ' ');

        if(parsed_line.size() != 2){
            std::cout << "Provided argument count mismatch at line: " << i + 1 << " (2 required, " << parsed_line.size() - 1 << " provided)\n";
            T0_CACHE_LINE_ERROR();
            result.clear();
            free(raw_cache.ptr);
            return result;
        }else{
            build_cache_line_t0 new_line;
            new_line.out_file_path = parsed_line[0];
            new_line.in_file_path = parsed_line[1];

            result.push_back(new_line);
        }
    }

    free(raw_cache.ptr);
    return result;
}

void update_build_cache_line_t0(build_cache_t0* cache, std::string in_file_path, std::string out_file_path){
    for(size_t i = 0; i < cache->size(); i++){
        if(cache->at(i).out_file_path == out_file_path){
            return;
        }
    }

    build_cache_line_t0 new_line;
    new_line.in_file_path = in_file_path;
    new_line.out_file_path = out_file_path;
    cache->push_back(new_line);
}

bool is_build_cache_line_t0_valid(build_cache_t0* cache, std::string in_file_path){
    build_cache_line_t0 line;

    for(size_t i = 0; i < cache->size(); i++){
        if(cache->at(i).in_file_path == in_file_path){
            line = cache->at(i);

            for(size_t j = 0; j < line.d.dependencies.size(); j++){
                if(get_file_last_update(line.d.dependencies[j]) > get_file_last_update(line.out_file_path)){
                    std::cout << "Dependency \"" << line.d.dependencies[j] << "\" is newer than output file \"" << line.out_file_path << "\"\n";
                    return false;
                }
            }
            
            return !(get_file_last_update(line.in_file_path) > get_file_last_update(line.out_file_path));
        }
    }
    return false;
}

void write_build_cache_t0(build_cache_t0* cache, std::string path){
    std::string cache_str = "";

    for(size_t i = 0; i < cache->size(); i++){
        cache_str +=  "\"" + cache->at(i).out_file_path + "\" \"" + cache->at(i).in_file_path + "\"\n";
    }

    Buffer raw_cache;
    raw_cache.ptr = cache_str.data();
    raw_cache.size = cache_str.size();

    write_file(path.c_str(), raw_cache);
}

std::string working_folder = std::filesystem::current_path().generic_string();

void _start_build_(std::string build_script){
    string_list lines = parse_with(build_script, '\n');

    uint64_t required_version = 0; // 0 == no version limitation

    std::vector<filter_var> filters;
    std::vector<list_var> lists;

    // check gcc exists
    command_output gcc_out = run_command("gcc --version");

    if(gcc_out.return_code != 0){
        std::cout << "gcc return code: " << gcc_out.return_code << "\n" << pretty_string_list(gcc_out.out) << "\n";
        GCC_ERROR();
    }

    // check gpp exists
    command_output gpp_out = run_command("g++ --version");

    if(gpp_out.return_code != 0){
        std::cout << "g++ return code: " << gpp_out.return_code << "\n" << pretty_string_list(gpp_out.out) << "\n";
        GPP_ERROR();
    }

    for(size_t i = 0; i < lines.size(); i++){
        std::string line = lines[i];
        
        line = remove_chars(line, "\n\r");
        
        if(line.empty()){
            continue; // skip
        }

        string_list parsed_line = xparse_with(line, ' ');

        if(parsed_line[0] == "VER"){
            if(parsed_line.size() != 2){
                std::cout << "Provided argument count mismatch at line: " << i + 1 << " (1 required, " << parsed_line.size() - 1 << " provided)\n";
                uBScript_PROVIDED_ARGUMENT_COUNT_MISMATCH_ERROR();
            }else{
                try{
                    required_version = std::stoull(parsed_line[1]);
                }catch(std::exception &e){
                    std::cout << "Provided parameter type error at line: " << i + 1 << " (ull required, string? provided)\n";
                    uBScript_PARAMETER_TYPE_ERROR();
                }
            }
            if(uVRB_version_number < required_version){
                std::cout << "Required version: " << required_version << " Current version: " << uVRB_version_number << "\n";
                uBScript_REQUIRED_VERSION_IS_HIGHER_ERROR();
            }
        }else if(parsed_line[0] == "FILTER"){
            if(parsed_line.size() != 2){
                std::cout << "Provided argument count mismatch at line: " << i + 1 << " (1 required, " << parsed_line.size() - 1 << " provided)\n";
                uBScript_PROVIDED_ARGUMENT_COUNT_MISMATCH_ERROR();
            }else{
                // check doesnt exists
                for(int j = 0; j < filters.size(); j++){
                    if(filters[j].var_name == parsed_line[1]){
                        std::cout << "FILTER redefinition at line: " << i + 1 << "\n";
                        uBScrpit_FILTER_ALREADY_DEFINED_ERROR();
                    }
                }  

                filter_var new_flt_var;
                new_flt_var.var_name = parsed_line[1];
                filters.push_back(new_flt_var);
            }
        }else if(parsed_line[0] == "INCEXT"){
            if(parsed_line.size() != 3){
                std::cout << "Provided argument count mismatch at line: " << i + 1 << " (2 required, " << parsed_line.size() - 1 << " provided)\n";
                uBScript_PROVIDED_ARGUMENT_COUNT_MISMATCH_ERROR();
            }else{
                size_t filter_index = 0;
                bool found = false;
                for(; filter_index < filters.size(); filter_index++){
                    if(filters[filter_index].var_name == parsed_line[1]){
                        found = true;
                        break;
                    }
                }

                if(!found){
                    std::cout << "Filter \"" << parsed_line[1] << "\" at line: " << i + 1 << " is not defined\n";
                    uBScript_FILTER_IS_NOT_DEFINED_ERROR();
                }else{
                    filters[filter_index].flt.included_extensions.push_back(parsed_line[2]);
                }
            }
        }else if(parsed_line[0] == "EXCDIR"){
            if(parsed_line.size() != 3){
                std::cout << "Provided argument count mismatch at line: " << i + 1 << " (2 required, " << parsed_line.size() - 1 << " provided)\n";
                uBScript_PROVIDED_ARGUMENT_COUNT_MISMATCH_ERROR();
            }else{
                size_t filter_index = 0;
                bool found = false;
                for(; filter_index < filters.size(); filter_index++){
                    if(filters[filter_index].var_name == parsed_line[1]){
                        found = true;
                        break;
                    }
                }

                if(!found){
                    std::cout << "Filter \"" << parsed_line[1] << "\" at line: " << i + 1 << " is not defined\n";
                    uBScript_FILTER_IS_NOT_DEFINED_ERROR();
                }else{
                    filters[filter_index].flt.excluded_directories.push_back(parsed_line[2]);
                }
            }
        }else if(parsed_line[0] == "LIST"){
            if(parsed_line.size() != 4){
                std::cout << "Provided argument count mismatch at line: " << i + 1 << " (3 required, " << parsed_line.size() - 1 << " provided)\n";
                uBScript_PROVIDED_ARGUMENT_COUNT_MISMATCH_ERROR();
            }else{
                // check doesnt exists
                for(int j = 0; j < lists.size(); j++){
                    if(lists[j].var_name == parsed_line[1]){
                        std::cout << "LIST redefinition at line: " << i + 1 << "\n";
                        uBScript_LIST_ALREADY_DEFINED_ERROR();
                    }
                }                

                list_var new_list;
                new_list.var_name = parsed_line[1];
                try{
                    if(parsed_line[2] == "ALL"){ // list all
                        new_list.list = list_sub_files(parsed_line[3].c_str());
                    }else{ // list with provided filter var
                        size_t filter_index = 0;
                        bool found = false;
                        for(; filter_index < filters.size(); filter_index++){
                            if(filters[filter_index].var_name == parsed_line[2]){
                                found = true;
                                break;
                            }
                        }

                        if(!found){
                            std::cout << "Filter \"" << parsed_line[2] << "\" at line: " << i + 1 << " is not defined\n";
                            uBScript_FILTER_IS_NOT_DEFINED_ERROR();
                        }else{
                            new_list.list = list_sub_files_with_filter(parsed_line[3].c_str(), filters[filter_index].flt);
                        }
                    }
                }catch(std::exception &e){
                    std::cout << "File access error: \"" << e.what() << "\" at line: " << i + 1 << "\n";
                    GENERATED_FILE_ACCESS_ERROR();
                }

                // listing complete
                lists.push_back(new_list);

                // debug std::cout << "LIST: " << new_list.var_name << ":\n" << pretty_string_list(new_list.list) << "\n";
            }
        }else if(parsed_line[0] == "KEEP"){
             if(parsed_line.size() != 3){
                std::cout << "Provided argument count mismatch at line: " << i + 1 << " (2 required, " << parsed_line.size() - 1 << " provided)\n";
                uBScript_PROVIDED_ARGUMENT_COUNT_MISMATCH_ERROR();
            }else{
                if(parsed_line[1] == "changed"){
                    size_t list_index = 0;
                    bool found = false;
                    for(; list_index < lists.size(); list_index++){
                        if(lists[list_index].var_name == parsed_line[2]){
                            found = true;
                            break;
                        }
                    }

                    if(!found){
                        std::cout << "List \"" << parsed_line[2] << "\" at line: " << i + 1 << " is not defined\n";
                        uBScript_LIST_NOT_DEFINED_ERROR();
                    }

                    // read cache
                    build_cache_t0 build_cache = read_build_cache_t0(working_folder +"/build/cache/cache_t0.uvrbcache");

                    // read dependencies
                    filter dependency_file_filter;
                    dependency_file_filter.included_extensions.push_back(".d");
                    try{
                        string_list dependency_files = list_sub_files_with_filter("build/objects/", dependency_file_filter);

                        for(size_t j = 0; j < dependency_files.size(); j++){
                            DependencyFile d = parse_dependency_file(dependency_files[j]);

                            for(size_t k = 0; k < build_cache.size(); k++){
                                if(build_cache[k].out_file_path == d.target){
                                   build_cache[k].d = d; // set dependency
                                }
                            }
                        }
                    }catch(std::exception &e){
                        std::cout << "Can not read dependency files: \"" << e.what() << "\" at line: " << i + 1 << "\n";
                    }

                    // compare change times & dependencies with cache
                    for(size_t j = 0; j < lists[list_index].list.size(); j++){
                        if(is_build_cache_line_t0_valid(&build_cache, lists[list_index].list[j])){
                            std::cout << "Skipping \"" << lists[list_index].list[j] << "\" (Not modified)\n";
                            lists[list_index].list.erase(lists[list_index].list.begin() + j); // remove from list
                            j--; // adjust index
                        }
                    }

                }
            }
        }else if(parsed_line[0] == "COMPILE++"){
            if(parsed_line.size() != 4){
                std::cout << "Provided argument count mismatch at line: " << i + 1 << " (3 required, " << parsed_line.size() - 1 << " provided)\n";
                uBScript_PROVIDED_ARGUMENT_COUNT_MISMATCH_ERROR();
            }else{
                size_t list_index = 0;
                bool found = false;
                for(; list_index < lists.size(); list_index++){
                    if(lists[list_index].var_name == parsed_line[2]){
                        found = true;
                        break;
                    }
                }

                // check doesnt exists
                for(int j = 0; j < lists.size(); j++){
                    if(lists[j].var_name == parsed_line[1]){
                        std::cout << "LIST redefinition at line: " << i + 1 << "\n";
                        uBScript_LIST_ALREADY_DEFINED_ERROR();
                    }
                }

                list_var out_list;
                out_list.var_name = parsed_line[1];

                if(!found){
                    std::cout << "List \"" << parsed_line[2] << "\" at line: " << i + 1 << " is not defined\n";
                    uBScript_LIST_NOT_DEFINED_ERROR();
                }

                std::filesystem::create_directories("build/objects");
                std::filesystem::create_directories("build/cache");

                build_cache_t0 build_cache = read_build_cache_t0(working_folder +"/build/cache/cache_t0.uvrbcache");

                for(size_t j = 0; j < lists[list_index].list.size(); j++){
                    std::string command = "g++ -c \"" + lists[list_index].list[j] + "\" -o \"" + working_folder + "/build/objects/" + get_file_name(lists[list_index].list[j]) + ".o\" " + parsed_line[3]; // 3th argument is other arguments
                    command_output gpp_compile_out = run_command(command.c_str());

                    std::cout << "Compiling: \"" << lists[list_index].list[j] << "\"...\n";

                    if(gpp_compile_out.return_code != 0){
                        std::cout << "g++ return code: " << gpp_compile_out.return_code << "\n" << pretty_string_list(gpp_compile_out.out) << "\n";
                        GPP_COMPILE_ERROR();
                    }

                    // add to compiled list
                    out_list.list.push_back(working_folder + "/build/objects/" + get_file_name(lists[list_index].list[j]) + ".o");

                    // update cache
                    update_build_cache_line_t0(&build_cache, lists[list_index].list[j], out_list.list[out_list.list.size() - 1]);
                }

                // add cached output files to output
                for(size_t j = 0; j < build_cache.size(); j++){
                    bool found = false;
                    for(size_t k = 0; k < out_list.list.size(); k++){
                        if(out_list.list[k] == build_cache[j].out_file_path){
                            found = true;
                            break;
                        }
                    }

                    if(!found){
                        out_list.list.push_back(build_cache[j].out_file_path);
                    }
                }

                lists.push_back(out_list);
                write_build_cache_t0(&build_cache, working_folder +"/build/cache/cache_t0.uvrbcache");

            }
        }else if(parsed_line[0] == "LINKX"){
            if(parsed_line.size() != 4){
                std::cout << "Provided argument count mismatch at line: " << i + 1 << " (3 required, " << parsed_line.size() - 1 << " provided)\n";
                uBScript_PROVIDED_ARGUMENT_COUNT_MISMATCH_ERROR();
            }

            // get compiled files first
            size_t list_index = 0;
            bool found = false;
            for(; list_index < lists.size(); list_index++){
                if(lists[list_index].var_name == parsed_line[1]){
                    found = true;
                    break;
                }
            }

            if(!found){
                    std::cout << "List \"" << parsed_line[1] << "\" at line: " << i + 1 << " is not defined\n";
                    uBScript_LIST_NOT_DEFINED_ERROR();
            }

            std::string command = "g++ " + unpretty_string_list(lists[list_index].list) + " -o \"" + working_folder + "/build/" + parsed_line[2] + "\" " + parsed_line[3];
            command_output ld_link_out = run_command(command.c_str());

            std::cout << "Linking...\n";

            if(ld_link_out.return_code != 0){
                std::cout << "ld return code: " << ld_link_out.return_code << "\n" << pretty_string_list(ld_link_out.out) << "\n";
                LINKER_ERROR();
            }

            std::cout << "Linked!\n";

        }else{
            std::cout << "Unknown command \"" << parsed_line[0] << "\" at line: " << i + 1 << "\n";
            uBScript_UNKNOWN_COMAND_ERROR();
        }
        
    }

}

int main(int argc, char *argv[]) {
    root._init_root_exclusive_();

    root["vayRUS"]["Builder"].add_child_str("version", uVRB_version_str);
    root["vayRUS"]["Builder"].add_child_str("host_version", uVRB_host_version_str);
    root["vayRUS"]["Builder"]["current"].add_child_str("working_folder", working_folder);
    
    std::cout << root["vayRUS"]["Builder"]["version"].pretty() << "\n";
    std::cout << root["vayRUS"]["Builder"]["host_version"].pretty() << "\n\n";

    // create argument holder
    root["vayRUS"]["Builder"]["const"]["command_line"]["argument"];

    bool stacking = false;

    std::string last_arg = "COMMAND";
    size_t attachment_counter = 0;

    for(int i = 0; i < argc; i++){
        if(argv[i][0] == '-'){
            root["vayRUS"]["Builder"]["const"]["command_line"]["argument"][argv[i]];
            last_arg = argv[i];
            attachment_counter = 0;
        }else{
            root["vayRUS"]["Builder"]["const"]["command_line"]["argument"][last_arg.c_str()].add_child_str("attachment" + std::to_string(attachment_counter), argv[i]);
            attachment_counter++;
        }
    }

    if(root["vayRUS"]["Builder"]["const"]["command_line"]["argument"].exists("--dd-uvar")){
        std::cout << "uVar debug dump:" << "\n";
        std::cout << root.pretty() << "\n\n";
    }

    if(!root.exists("vayRUS.Builder.const.command_line.argument.COMMAND.attachment1")){
        NO_COMMAND_PROVIDED_ERROR();
    }

    if(root["vayRUS"]["Builder"]["const"]["command_line"]["argument"]["COMMAND"]["attachment1"].str_value == "build"){
        Buffer raw_uBuild;

        if(root.exists("vayRUS.Builder.const.command_line.argument.-u.attachment0")){
            raw_uBuild = read_file(root["vayRUS"]["Builder"]["const"]["command_line"]["argument"]["-u"]["attachment0"].str_value.c_str());
            if(raw_uBuild.ptr == nullptr){
                std::cout << "Can not open: " << root["vayRUS"]["Builder"]["const"]["command_line"]["argument"]["-u"]["attachment0"].str_value << "\n";
                PROVIDED_FILE_ACCESS_ERROR();
            }
        }else{
            std::cout << "no uBuild.u provided!" << "\n";

            raw_uBuild = read_file("uBuild.u");
            if(raw_uBuild.ptr == nullptr){
                std::cout << "Can not open: " << "uBuild.u" << "\n";
                GENERATED_FILE_ACCESS_ERROR();
            }
        }

        std::string build_script = std::string((char*)raw_uBuild.ptr, (char*)raw_uBuild.ptr + raw_uBuild.size);

        _start_build_(build_script);

        std::cout << "*** Build successful! ***\n";

        return 0;
    }
    else if(root["vayRUS"]["Builder"]["const"]["command_line"]["argument"]["COMMAND"]["attachment1"].str_value == "help"){
        std::cout << uVRB_help_str;
        return 0;
    }
    std::cout << "Unknown command: \"" << root["vayRUS"]["Builder"]["const"]["command_line"]["argument"]["COMMAND"]["attachment1"].str_value << "\"\n";
    UNKNOWN_COMMAND_ERROR();

    filter flt;
    flt.included_extensions.push_back(".txt");

    string_list test = list_sub_files_with_filter(root["vayRUS"]["Builder"]["const"]["command_line"]["argument"]["--sd"]["attachment0"].str_value.c_str(), flt);
    std::cout << pretty_string_list(test);
}