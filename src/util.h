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
std::string           day_file(uint8_t day);
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

namespace util {
template<size_t AX_ROWS, size_t AX_COLS>
class Grid {
    static constexpr size_t          WIDTH{AX_COLS};
    static constexpr size_t          HEIGHT{AX_ROWS};
    std::array<char, HEIGHT * WIDTH> m_buffer{0};

    static size_t idx(size_t row, size_t col) {
        if (row >= height()) {
            throw std::runtime_error(
                std::format("row {} out of bounds, max: {}", row, height())
            );
        }
        if (col >= width()) {
            throw std::runtime_error(
                std::format("col {} out of bounds, max: {}", col, width())
            );
        }
        return (row * WIDTH) + col;
    }

    char& get_mut(size_t row, size_t col) {
        return m_buffer[idx(row, col)]; // NOLINT
    }

public:
    explicit Grid(std::ifstream& infile) {
        std::string line;
        for (size_t row{0}; row < height(); ++row) {
            std::getline(infile, line);
            for (size_t col{0}; col < width(); ++col) {
                get_mut(row, col) = line[col];
            }
        }
    }

    [[nodiscard]]
    constexpr size_t size() const {
        return m_buffer.size();
    }

    [[nodiscard]]
    const char& get_item(size_t row, size_t col) const {
        return m_buffer[idx(row, col)]; // NOLINT
    }

    [[nodiscard]]
    bool check(size_t row, size_t col) {
        return (row < height() && col < width());
    }

    static constexpr size_t width() {
        return WIDTH;
    }

    static constexpr size_t height() {
        return HEIGHT;
    }

    explicit operator std::string() const {
        std::string outstr{};
        for (size_t row{0}; row < height(); ++row) {
            for (size_t col{0}; col < width(); ++col) {
                char item = get_item(row, col);
                outstr.push_back(item);
            }
            outstr.push_back('\n');
        }
        return outstr;
    }
};
} // namespace util

} // namespace aoc
