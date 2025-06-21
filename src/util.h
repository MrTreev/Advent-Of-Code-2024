#pragma once
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cxxabi.h>
#include <filesystem>
#include <format>
#include <fstream>
#include <mdspan>
#include <source_location>
#include <sstream> // IWYU pragma: keep
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
constexpr bool test_mode =
#if defined(NDEBUG)
    false
#else
    true
#endif
    ;

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
extern std::atomic<size_t> part1;
extern std::atomic<size_t> part2;
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

void run();
void print(const std::string& str);

void print(std::string_view fst, auto&&... args) {
    print(std::string(std::vformat(fst, std::make_format_args(args...))));
}

void debug(std::string_view fst, auto&&... args) {
    if constexpr (test_mode) {
        print(fst, std::forward<decltype(args)>(args)...);
    }
}

bool in(auto item, const std::vector<decltype(item)>& vec) {
    return std::ranges::any_of(vec.begin(), vec.end(), [item](auto cmp) {
        return cmp == item;
    });
}

namespace file {
std::string           day_file(uint8_t day);
std::filesystem::path day_path(uint8_t day);
std::filesystem::path
day_stream(const std::source_location& loc = std::source_location::current());
} // namespace file

namespace string {
std::string slurp(std::ifstream& instr);

bool    is_numeric(const std::string& str);
uint8_t char_to_uint(char cha);
uint8_t char_to_uint(char cha_1, char cha_2);
} // namespace string

namespace math {
size_t max(size_t aval, size_t bval);
size_t min(size_t aval, size_t bval);
size_t max(size_t aval, size_t bval, size_t args, ...);
size_t min(size_t aval, size_t bval, size_t args, ...);
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
enum class Direction : uint8_t { N, E, S, W };
std::string to_string(Direction dir);

class Coord {
    size_t m_xpos;
    size_t m_ypos;

public:
    Coord(size_t xpos, size_t ypos);
    [[nodiscard]]
    size_t x() const;
    [[nodiscard]]
    size_t   y() const;
    explicit operator std::string() const;
    bool     operator<=>(const Coord& other) const = default;
    Coord    operator+(const Direction& dir) const;
    Coord    operator-(const Coord& other) const;
    Coord    operator+(const Coord& other) const;
    Coord    operator*(const size_t& other) const;
    Coord    operator*(const Coord& other) const;
    void     operator-=(const Coord& other);
    void     operator+=(const Coord& other);
    void     operator/=(const Coord& other);
    void     operator*=(const Coord& other);
};

class Grid {
private:
    size_t                                      m_width{0};
    size_t                                      m_height{0};
    std::vector<char>                           m_buffer;
    std::mdspan<char, std::dextents<size_t, 2>> m_mdspan;

public:
    explicit Grid() = default;
    explicit Grid(size_t len);
    explicit Grid(size_t width, size_t height);
    explicit Grid(std::ifstream& infile, size_t len);
    Grid(const Grid& other);
    Grid(Grid&& other) noexcept;
    Grid& operator=(const Grid&) = default;
    Grid& operator=(Grid&&)      = default;
    ~Grid()                      = default;
    explicit operator std::string() const;
    auto&&   operator[](this auto&& self, size_t xpos, size_t ypos);
    auto&&   operator[](this auto&& self, Coord pos);
    [[nodiscard]]
    size_t width() const;
    [[nodiscard]]
    size_t height() const;
    [[nodiscard]]
    size_t elements() const;
    [[nodiscard]]
    bool check(Coord pos) const;
};

inline auto&& Grid::operator[](this auto&& self, size_t xpos, size_t ypos) {
    return self.m_mdspan[ypos, xpos];
}

inline auto&& Grid::operator[](this auto&& self, Coord pos) {
    return self[pos.x(), pos.y()];
}

} // namespace util

} // namespace aoc
