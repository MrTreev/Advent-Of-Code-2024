#pragma once
#include <algorithm>
#include <atomic>
#include <cassert>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cxxabi.h>
#include <filesystem>
#include <format>
#include <fstream>
#include <string>
#include <typeinfo>
#include <vector>

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
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
extern std::atomic<size_t> part1;
extern std::atomic<size_t> part2;
extern std::atomic<bool>   test_mode;
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

void run(); // Use this to run your day's code
void print(const std::string& str);
void debug(const std::string& str);

void print(std::string_view fst, auto&&... args) {
    print(std::string(std::vformat(fst, std::make_format_args(args...))));
}

void debug(std::string_view fst, auto&&... args) {
    debug(std::string(std::vformat(fst, std::make_format_args(args...))));
}

bool in(auto item, const std::vector<decltype(item)>& vec) {
    return std::ranges::any_of(vec.begin(), vec.end(), [item](auto cmp) {
        return cmp == item;
    });
}

namespace file {
std::string           day_txt(uint8_t day);
std::filesystem::path day_path(uint8_t day);
std::ifstream         day_stream(uint8_t day);
} // namespace file

namespace output {
void print_part(uint8_t num, const size_t& part);
} // namespace output

namespace string {
std::string slurp(std::ifstream instr);

bool    is_numeric(const std::string& str);
uint8_t char_to_uint(char cha);
uint8_t char_to_uint(char cha_1, char cha_2);
} // namespace string

namespace math {
size_t max(size_t aval, size_t bval);
size_t min(size_t aval, size_t bval);
size_t max(size_t aval, size_t bval, size_t args...);
size_t min(size_t aval, size_t bval, size_t args...);
} // namespace math

namespace types {
std::string type_name(auto item) {
    char* demangled_name =
        abi::__cxa_demangle(typeid(item).name(), nullptr, nullptr, nullptr);
    std::string ret_name{demangled_name};
    free(demangled_name); //NOLINT(*-no-malloc,cppcoreguidelines-owning-memory)
    return ret_name;
}
} // namespace types

} // namespace aoc
