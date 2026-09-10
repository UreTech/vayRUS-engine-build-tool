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
DependencyFile parse_dependency_file(const std::string& path)
{
    DependencyFile result;

    std::ifstream file(path);
    if (!file)
    {
        std::cout << ".d file read error!\n";
        return result;
    }

    std::string text(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    std::string normalized;
    normalized.reserve(text.size());

    for (size_t i = 0; i < text.size(); ++i)
    {
        if (text[i] == '\r')
            continue;

        normalized += text[i];
    }

    std::string joined;
    joined.reserve(normalized.size());

    for (size_t i = 0; i < normalized.size(); ++i)
    {
        if (normalized[i] == '\\' &&
            i + 1 < normalized.size() &&
            normalized[i + 1] == '\n')
        {
            joined += ' ';
            ++i;
        }
        else
        {
            joined += normalized[i];
        }
    }

    size_t colon = std::string::npos;
    bool escaped = false;

    for (size_t i = 0; i < joined.size(); ++i)
    {
        char c = joined[i];

        if (escaped)
        {
            escaped = false;
            continue;
        }

        if (c == '\\')
        {
            escaped = true;
            continue;
        }

        if (c == ':')
        {
            colon = i;
            break;
        }
    }

    if (colon == std::string::npos)
    {
        std::cout << "No ':' found!\n";
        return result;
    }

    result.target = joined.substr(0, colon);

    while (!result.target.empty() &&
           std::isspace(static_cast<unsigned char>(result.target.back())))
    {
        result.target.pop_back();
    }

    std::string deps = joined.substr(colon + 1);

    std::vector<std::string> tokens;
    std::string current;

    escaped = false;

    for (char c : deps)
    {
        if (escaped)
        {
            current += c;
            escaped = false;
            continue;
        }

        if (c == '\\')
        {
            escaped = true;
            continue;
        }

        if (c == ' ' || c == '\t' || c == '\n')
        {
            if (!current.empty())
            {
                tokens.push_back(current);
                current.clear();
            }

            continue;
        }

        current += c;
    }

    if (escaped)
        current += '\\';

    if (!current.empty())
        tokens.push_back(current);

    result.dependencies = std::move(tokens);

    return result;
}