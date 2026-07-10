#include <uVar/uVar.h>
#include <util/util.h>
// root
uVar root;

// uVar
uVar::uVar(){
    name = "u";
    parent = nullptr;
    type_id = uVar_TYPE_ONLY_BRANCH;
}

uVar& uVar::operator[](const char* str) {
    for(size_t i = 0; i < children.size(); i++){
        if(std::string(str) == children[i]->name){
            return *children[i];
        }
    }

    this->add_child(str); // add child if not found
    return *children[children.size() - 1];
}

bool uVar::exists(std::string uVar_entry){
    string_list path = parse_with(uVar_entry, '.');
    uVar *current = this;

    for(size_t j = 0; j < path.size(); j++){

        if(current == nullptr){
            return false;
        }

        bool pass = false;
        for(size_t i = 0; i < current->children.size(); i++){
            if(path[j] == current->children[i]->name){
                current = current->children[i];
                pass = true;
                break; // this entry exists
            }
        }

        if(!pass){
            return false; // check failed
        }
    }
    return true; // check succeed
}

void uVar::_init_root_exclusive_(){
    root.name = "u";
    root.type_id = uVar_TYPE_ONLY_BRANCH;
}

std::string uVar::pretty(size_t offset){
    std::string parent_path = name;
    
    uVar* up = parent;
    while(up != nullptr){
        std::string tmp = up->name;
        tmp.push_back('.');
        parent_path = tmp + parent_path;
        up = up->parent;
    }

    std::string result = "";

    std::string child_result = "";
    for(size_t i = 0; i < children.size();i++){
        child_result += children[i]->pretty() + "\n";
    }
    if(!child_result.empty()){
        child_result.pop_back();
    }

    switch(type_id){
        case uVar_TYPE_ONLY_BRANCH:
            if(child_result.empty()){
                return parent_path;
            }else{
                return child_result;
            }
        case uVar_TYPE_STRING:
            result = parent_path + " = (STRING) \"" + str_value + "\"";
            if(!child_result.empty()){
                return result + "\n" + child_result;
            }else{
                return result;
            }

        case uVar_TYPE_ULL:
            result = parent_path + " = (ULL) \"" + std::to_string(ull_value) + "\"";
            if(!child_result.empty()){
                return result + "\n" + child_result;
            }else{
                return result;
            }

        case uVar_TYPE_SLL:
            result = parent_path + " = (SLL) \"" + std::to_string(sll_value) + "\"";
            if(!child_result.empty()){
                return result + "\n" + child_result;
            }else{
                return result;
            }

        case uVar_TYPE_FLOAT:
            result = parent_path + " = (FLOAT) \"" + std::to_string(float_value) + "\"";
            if(!child_result.empty()){
                return result + "\n" + child_result;
            }else{
                return result;
            }

        default:
            result = parent_path + " = UNKNOWN TYPE: " + std::to_string(type_id) + " name: " +  name;
            if(!child_result.empty()){
                return result + "\n" + child_result;
            }else{
                return result;
            }    
    }
}
void uVar::add_child(std::string _name){
    uVar *child = new uVar();
    child->name = _name;
    child->parent = this;
    child->type_id = uVar_TYPE_ONLY_BRANCH;
    children.push_back(child);
}

void uVar::add_child_str(std::string _name, std::string _value){
    uVar *child = new uVar();
    child->name = _name;
    child->parent = this;
    child->str_value = _value;
    child->type_id = uVar_TYPE_STRING;
    children.push_back(child);
}
void uVar::add_child_ull(std::string _name, unsigned long long _value){
    uVar *child = new uVar();;
    child->name = _name;
    child->parent = this;
    child->ull_value = _value;
    child->type_id = uVar_TYPE_ULL;
    children.push_back(child);
}
void uVar::add_child_sll(std::string _name, signed long long _value){
    uVar *child = new uVar();
    child->name = _name;
    child->parent = this;
    child->sll_value = _value;
    child->type_id = uVar_TYPE_SLL;
    children.push_back(child);
}
void uVar::add_child_float(std::string _name, float _value){
    uVar *child = new uVar();
    child->name = _name;
    child->parent = this;
    child->float_value = _value;
    child->type_id = uVar_TYPE_FLOAT;
    children.push_back(child);
}
