#ifndef uVRB_config_H
#define uVRB_config_H

#include <cstdint>

#define uVRB_version_str "uvrb1.1.2 QV2"

#define uVRB_help_str "UreTech vayRUS Build Tool Command Line Arguments:\n COMMANDS:\n  |-> build: Start build with default build script \"uBuild.u\"\n  |-> help: Print this help message\n ARGUMENTS:\n  |-> -u [build_script.u]: Build script file\n  |-> --dd-uvar: Print uVar debug\n"

#define uVRB_version_number 112ULL

#define ONEBIT(n) (1ULL << n)

#include <iostream>
#define MDBG(msg) std::cout << __FILE__ << ":" << __LINE__ << " --> \"" << msg << "\"\n";

#ifdef _WIN32
#define uVRB_host_version_str "uHOST.WIN32"
#elif _WIN64
#define uVRB_host_version_str "uHOST.WIN64"
#elif __linux__
#define uVRB_host_version_str "uHOST.LINUX"
#elif __ANDROID__
#define uVRB_host_version_str "uHOST.ANDROID"
#elif __uVRB_CUSTOM_HOST__
#define uVRB_host_version_str "uHOST.CUSTOM"
#else
#define uVRB_host_version_str "uHOST.UNDEFINED"
#endif

#define ERROR_REPORT(UEC_ERROR_CODE, UED_ERROR_DESCRIPTION) std::cerr << "UEC Code: [" << UEC_ERROR_CODE << "]: " << UED_ERROR_DESCRIPTION << "\n";

#define UEC_UNKNOWN_ERROR (100ULL)
#define UED_UNKNOWN_ERROR "This is worst thing could happen :("
#define UNKNOWN_ERROR() ERROR_REPORT(UEC_UNKNOWN_ERROR, UED_UNKNOWN_ERROR); exit(UEC_UNKNOWN_ERROR);

#define UEC_INTERNAL_ERROR (101ULL)
#define UED_INTERNAL_ERROR "This is second worst thing could happen :("
#define INTERNAL_ERROR() ERROR_REPORT(UEC_INTERNAL_ERROR, UED_INTERNAL_ERROR); exit(UEC_INTERNAL_ERROR);

#define UEC_GCC_ERROR (102ULL)
#define UED_GCC_ERROR "No gcc installation found or env variables are not set"
#define GCC_ERROR() ERROR_REPORT(UEC_GCC_ERROR, UED_GCC_ERROR); exit(UEC_GCC_ERROR);

#define UEC_GPP_ERROR (103ULL)
#define UED_GPP_ERROR "No g++ installation found or env variables are not set"
#define GPP_ERROR() ERROR_REPORT(UEC_GPP_ERROR, UED_GPP_ERROR); exit(UEC_GPP_ERROR);

/* NOTE: Cache errors are not critical so we are not exiting with error a code */

#define UEC_UNKNOWN_CACHE_ERROR (200ULL)
#define UED_UNKNOWN_CACHE_ERROR "An unknown cache error has occured. Maybe cache file is corrupted or not compatible with current version"
#define UNKNOWN_CACHE_ERROR() ERROR_REPORT(UEC_UNKNOWN_CACHE_ERROR, UED_UNKNOWN_CACHE_ERROR);

#define UEC_T0_CACHE_LINE_ERROR (201ULL)
#define UED_T0_CACHE_LINE_ERROR "T0 cache line is corrupted or not compatible with current version"
#define T0_CACHE_LINE_ERROR() ERROR_REPORT(UEC_T0_CACHE_LINE_ERROR, UED_T0_CACHE_LINE_ERROR);

#define UEC_PROVIDED_FILE_ACCESS_ERROR (1000ULL)
#define UED_PROVIDED_FILE_ACCESS_ERROR "Provided file access error"
#define PROVIDED_FILE_ACCESS_ERROR() ERROR_REPORT(UEC_PROVIDED_FILE_ACCESS_ERROR, UED_PROVIDED_FILE_ACCESS_ERROR); exit(UEC_PROVIDED_FILE_ACCESS_ERROR);

#define UEC_GENERATED_FILE_ACCESS_ERROR (1001ULL)
#define UED_GENERATED_FILE_ACCESS_ERROR "Generated file access error"
#define GENERATED_FILE_ACCESS_ERROR() ERROR_REPORT(UEC_GENERATED_FILE_ACCESS_ERROR, UED_GENERATED_FILE_ACCESS_ERROR); exit(UEC_GENERATED_FILE_ACCESS_ERROR);

#define UEC_NO_COMMAND_PROVIDED_ERROR (1002ULL)
#define UED_NO_COMMAND_PROVIDED_ERROR "No command provided"
#define NO_COMMAND_PROVIDED_ERROR() ERROR_REPORT(UEC_NO_COMMAND_PROVIDED_ERROR, UED_NO_COMMAND_PROVIDED_ERROR); exit(UEC_NO_COMMAND_PROVIDED_ERROR);

#define UEC_UNKNOWN_COMMAND_ERROR (1003ULL)
#define UED_UNKNOWN_COMMAND_ERROR "Unknown command"
#define UNKNOWN_COMMAND_ERROR() ERROR_REPORT(UEC_UNKNOWN_COMMAND_ERROR, UED_UNKNOWN_COMMAND_ERROR); exit(UEC_UNKNOWN_COMMAND_ERROR);

#define UEC_GPP_COMPILE_ERROR (1004ULL)
#define UED_GPP_COMPILE_ERROR "g++ compile failed"
#define GPP_COMPILE_ERROR() ERROR_REPORT(UEC_GPP_COMPILE_ERROR, UED_GPP_COMPILE_ERROR); exit(UEC_GPP_COMPILE_ERROR);

#define UEC_LINKER_ERROR (2001ULL)
#define UED_LINKER_ERROR "Linking failed"
#define LINKER_ERROR() ERROR_REPORT(UEC_LINKER_ERROR, UED_LINKER_ERROR); exit(UEC_LINKER_ERROR);

#define UEC_uBScript_UNKNOWN_COMAND_ERROR (2000ULL)
#define UED_uBScript_UNKNOWN_COMAND_ERROR "uBuild Script: Unknown command error"
#define uBScript_UNKNOWN_COMAND_ERROR() ERROR_REPORT(UEC_uBScript_UNKNOWN_COMAND_ERROR, UED_uBScript_UNKNOWN_COMAND_ERROR); exit(UEC_uBScript_UNKNOWN_COMAND_ERROR);

#define UEC_uBScript_PROVIDED_ARGUMENT_COUNT_MISMATCH_ERROR (2001ULL)
#define UED_uBScript_PROVIDED_ARGUMENT_COUNT_MISMATCH_ERROR "uBuild Script: Provided argument count is not matching"
#define uBScript_PROVIDED_ARGUMENT_COUNT_MISMATCH_ERROR() ERROR_REPORT(UEC_uBScript_PROVIDED_ARGUMENT_COUNT_MISMATCH_ERROR, UED_uBScript_PROVIDED_ARGUMENT_COUNT_MISMATCH_ERROR); exit(UEC_uBScript_PROVIDED_ARGUMENT_COUNT_MISMATCH_ERROR);

#define UEC_uBScript_PARAMETER_TYPE_ERROR (2002ULL)
#define UED_uBScript_PARAMETER_TYPE_ERROR "uBuild Script: Provided parameter type error"
#define uBScript_PARAMETER_TYPE_ERROR() ERROR_REPORT(UEC_uBScript_PARAMETER_TYPE_ERROR, UED_uBScript_PARAMETER_TYPE_ERROR); exit(UEC_uBScript_PARAMETER_TYPE_ERROR);

#define UEC_uBScript_REQUIRED_VERSION_IS_HIGHER_ERROR (2003ULL)
#define UED_uBScript_REQUIRED_VERSION_IS_HIGHER_ERROR "uBuild Script: Required builder version is higher than current"
#define uBScript_REQUIRED_VERSION_IS_HIGHER_ERROR() ERROR_REPORT(UEC_uBScript_REQUIRED_VERSION_IS_HIGHER_ERROR, UED_uBScript_REQUIRED_VERSION_IS_HIGHER_ERROR); exit(UEC_uBScript_REQUIRED_VERSION_IS_HIGHER_ERROR);

#define UEC_uBScript_FILTER_IS_NOT_DEFINED_ERROR (2004ULL)
#define UED_uBScript_FILTER_IS_NOT_DEFINED_ERROR "uBuild Script: Used filter is not defined"
#define uBScript_FILTER_IS_NOT_DEFINED_ERROR() ERROR_REPORT(UEC_uBScript_FILTER_IS_NOT_DEFINED_ERROR, UED_uBScript_FILTER_IS_NOT_DEFINED_ERROR); exit(UEC_uBScript_FILTER_IS_NOT_DEFINED_ERROR);

#define UEC_uBScrpit_FILTER_ALREADY_DEFINED_ERROR (2005ULL)
#define UED_uBScrpit_FILTER_ALREADY_DEFINED_ERROR "uBuild Script: Double FILTER definition is not allowed"
#define uBScrpit_FILTER_ALREADY_DEFINED_ERROR() ERROR_REPORT(UEC_uBScrpit_FILTER_ALREADY_DEFINED_ERROR, UED_uBScrpit_FILTER_ALREADY_DEFINED_ERROR); exit(UEC_uBScrpit_FILTER_ALREADY_DEFINED_ERROR);

#define UEC_uBScript_LIST_NOT_DEFINED_ERROR (2006ULL)
#define UED_uBScript_LIST_NOT_DEFINED_ERROR "uBuild Script: LIST is not defined"
#define uBScript_LIST_NOT_DEFINED_ERROR() ERROR_REPORT(UEC_uBScript_LIST_NOT_DEFINED_ERROR, UED_uBScript_LIST_NOT_DEFINED_ERROR); exit(UEC_uBScript_LIST_NOT_DEFINED_ERROR);

#define UEC_uBScript_LIST_ALREADY_DEFINED_ERROR (2007ULL)
#define UED_uBScript_LIST_ALREADY_DEFINED_ERROR "uBuild Script: Double LIST definition is not allowed"
#define uBScript_LIST_ALREADY_DEFINED_ERROR() ERROR_REPORT(UEC_uBScript_LIST_ALREADY_DEFINED_ERROR, UED_uBScript_LIST_ALREADY_DEFINED_ERROR); exit(UEC_uBScript_LIST_ALREADY_DEFINED_ERROR);

// util
struct Buffer{
    void* ptr = nullptr;
    size_t size = 0;
};

#include <string>
#include <vector>
typedef std::vector<std::string> string_list;

#endif