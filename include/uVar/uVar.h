#ifndef uVar_H
#define uVar_H

#include <string>
#include <vector>
#include <uVRB_config.h>
#include <cstdint>

#define uVar_FLAG_CONSTANT ONEBIT(0)

#define uVar_TYPE_ONLY_BRANCH 3ULL
#define uVar_TYPE_STRING 4ULL
#define uVar_TYPE_ULL 5ULL
#define uVar_TYPE_SLL 6ULL
#define uVar_TYPE_FLOAT 7ULL

class uVar{
public:
    uVar();

    std::string name = "null_node";
    uVar* parent = nullptr;
    uint64_t flags;
private:
    uint8_t type_id = 0xFF;
    std::vector<uVar*> children;

public:
    std::string str_value = ""; // uVar_TYPE_STRING
    unsigned long long ull_value = 0ULL; // uVar_TYPE_ULL
    signed long long sll_value = 0ULL; // uVar_TYPE_SLL
    float float_value = 0.0f; // uVar_TYPE_FLOAT
    
    uVar& operator[](const char* str);

    bool exists(std::string uVar_entry);
    
    void _init_root_exclusive_();

    void add_child(std::string _name);

    // variants
    void add_child_str(std::string _name, std::string _value);
    void add_child_ull(std::string _name, unsigned long long _value);
    void add_child_sll(std::string _name, signed long long _value);
    void add_child_float(std::string _name, float _value);
    
    // void remove_child(uVar* child);

    std::string pretty(size_t offset = 0);
};

extern uVar root;

#endif