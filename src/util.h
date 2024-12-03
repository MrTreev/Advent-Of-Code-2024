#pragma once
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cxxabi.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <typeinfo>

using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::int8_t;

using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::uint8_t;

using std::intmax_t;
using std::intptr_t;
using std::uintmax_t;
using std::uintptr_t;

using std::byte;
using std::nullptr_t;
using std::ptrdiff_t;
using std::size_t;

namespace aoc {
template<typename... Args>
void print(std::format_string<Args...> fst, Args... args) {
    std::println(std::cout, fst, std::forward<Args...>(args)...);
}

namespace file {
std::string           day_txt(uint8_t day);
std::filesystem::path day_path(uint8_t day);
std::ifstream         day_stream(uint8_t day);
} // namespace file

namespace output {
void print_part(uint8_t num, const auto& part) {
    std::println(std::cout, "Part {}: {}", num, part);
}
} // namespace output

namespace debug {
std::string type_name(auto item) {
    char* demangled_name =
        abi::__cxa_demangle(typeid(item).name(), nullptr, nullptr, nullptr);
    std::string ret_name{demangled_name};
    free(demangled_name);
    return ret_name;
}
} // namespace debug

namespace string {
void clean_string(std::string& str);
void remove_duplicates(std::string& str);
} // namespace string

} // namespace aoc
