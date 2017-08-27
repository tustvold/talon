#pragma once
#include <TalonConfig.hpp>
#include <string>

#if defined(__clang__) || defined(__GNUG__)
#include <cxxabi.h>
#endif

TALON_NS_BEGIN

template<typename T>
std::string type_name()
{
    std::string tname = typeid(T).name();
#if defined(__clang__) || defined(__GNUG__)
    int status;
    char *demangled_name = abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status);
    if(status == 0)
    {
        tname = demangled_name;
        std::free(demangled_name);
    }
#endif
    return tname;
}

TALON_NS_END