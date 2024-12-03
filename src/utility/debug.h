#pragma once
#include <cstdlib>
#include <cxxabi.h>
#include <string>
#include <typeinfo>

namespace core::debug {

template<typename T>
constexpr std::string type_name(T item) {
    char* demangled_name =
        abi::__cxa_demangle(typeid(item).name(), nullptr, nullptr, nullptr);
    std::string ret_name{demangled_name};
    free(demangled_name);
    return ret_name;
}

template<typename T>
constexpr std::string type_name() {
    const T item{};
    char*   demangled_name =
        abi::__cxa_demangle(typeid(item).name(), nullptr, nullptr, nullptr);
    std::string ret_name{demangled_name};
    free(demangled_name);
    return ret_name;
}

} // namespace core::debug
