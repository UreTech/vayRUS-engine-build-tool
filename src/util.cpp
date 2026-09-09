#include <util/util.h>

string_list parse_with(std::string input, char seperator){
    string_list result;

    std::string tmp = "";

    for(size_t i = 0; i < input.size(); i++){
        if(input[i] == seperator){
            if(!tmp.empty()){
                result.push_back(tmp);
                tmp.clear();
            }
        }else{
            tmp.push_back(input[i]);
        }
    }
    if(!tmp.empty()){
        result.push_back(tmp);
    }

    return result;
}

string_list xparse_with(std::string input, char seperator, char bracket){
    string_list result;

    std::string tmp = "";

    bool quote = false;

    for(size_t i = 0; i < input.size(); i++){

        if(input[i] == bracket){
            quote = !quote;
        }
        else if(input[i] == seperator && !quote){
            if(!tmp.empty()){
                result.push_back(tmp);
                tmp.clear();
            }
        }else{
            tmp.push_back(input[i]);
        }
    }
    if(!tmp.empty()){
        result.push_back(tmp);
    }

    return result;
}

std::string remove_chars(std::string input, std::string chrs){
    std::string result;
    
    for(size_t i = 0; i < input.size(); i++){
        bool skip = false;
        for(size_t j = 0; j < chrs.size(); j++){
            if(input[i] == chrs[j]){
                skip = true;
                break;
            }
        }

        if(!skip){
            result.push_back(input[i]);
        }
    }

    return result;
}

command_output run_command(const char* command){
    command_output result;


    FILE* pipe = popen(command, "r");

    if(pipe == nullptr){
        std::cout << "WARNING! Failed to run command \"" << command << "\"\n";
        result.return_code = -1;
        return result;
    }

    char buf[2048];

    while (fgets(buf, sizeof(buf), pipe))
    {
        result.out.push_back(std::string((const char*)buf));
    }

    result.return_code = pclose(pipe);

    return result;
}