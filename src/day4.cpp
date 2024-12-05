#include "util.h"
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#define AOC_UBU(x)                                                             \
    _Pragma("clang diagnostic push")                                           \
        _Pragma("clang diagnostic ignored \"-Wunsafe-buffer-usage\"")          \
            x _Pragma("clang diagnostic pop")

namespace {
#define AOC_TEST
#ifndef AOC_TEST
constexpr size_t WIDTH{140};
constexpr size_t HEIGHT{140};
#else
constexpr size_t WIDTH{10};
constexpr size_t HEIGHT{10};
#endif

enum Direction : uint8_t {
    N  = 0b1000,
    NE = 0b1010,
    E  = 0b0010,
    SE = 0b0110,
    S  = 0b0100,
    SW = 0b0101,
    W  = 0b0001,
    NW = 0b1001,
};

std::string to_string(Direction dir) {
    switch (dir) {
    case N:
        return "N ";
    case NE:
        return "NE";
    case E:
        return "E ";
    case SE:
        return "SE";
    case S:
        return "S ";
    case SW:
        return "SW";
    case W:
        return "W ";
    case NW:
        return "NW";
    }
}

constexpr std::array<Direction, 8> DIRECTIONS{
    {
     N, NE,
     E, SE,
     S, SW,
     W, NW,
     }
};

bool north(Direction dir) {
    return (dir & Direction::N) != 0;
}

bool south(Direction dir) {
    return (dir & Direction::S) != 0;
}

bool east(Direction dir) {
    return (dir & Direction::E) != 0;
}

bool west(Direction dir) {
    return (dir & Direction::W) != 0;
}

auto to_unsigned(auto val) {
    static_assert(std::is_signed_v<decltype(val)>);
    if (val < 0) {
        throw std::runtime_error(
            std::format("Could not make convert {} to unsigned", val)
        );
    }
    return static_cast<std::make_unsigned_t<decltype(val)>>(val);
}

class WordSearch {
private:
    std::array<char, WIDTH * HEIGHT> m_buffer{};

    static size_t idx(size_t row, size_t col) {
        if (std::cmp_greater(row, HEIGHT)) {
            throw std::runtime_error(
                std::format("row {} out of bounds, max: {}", row, HEIGHT)
            );
        }
        if (std::cmp_greater(col, WIDTH)) {
            throw std::runtime_error(
                std::format("col {} out of bounds, max: {}", col, WIDTH)
            );
        }
        return (static_cast<size_t>(row) * WIDTH) + static_cast<size_t>(col);
    }

    [[nodiscard]]
    std::string get_line(size_t row) const {
        return {&m_buffer[idx(row, 0)], WIDTH};
    }

public:
    WordSearch() {
        m_buffer.fill('.');
    }

    explicit WordSearch(std::ifstream& infile) {
        std::string line;
        size_t      lineno = 0;
        while (std::getline(infile, line)) {
            assert(line.size() == WIDTH);
            AOC_UBU(auto* bufiter = m_buffer.begin() + (lineno * WIDTH));
            std::copy(line.cbegin(), line.cend(), bufiter);
            ++lineno;
            for (size_t idx{0}; idx < m_buffer.size(); ++idx) {
                // NOLINTBEGIN(cppcoreguidelines-pro-bounds-constant-array-index)
                if (std::cmp_less(idx, (lineno * WIDTH))) {
                    assert(m_buffer[idx] != 0);
                } else {
                    assert(m_buffer[idx] == 0);
                }
                // NOLINTEND(cppcoreguidelines-pro-bounds-constant-array-index)
            }
        }
    }

    explicit operator std::string() {
        std::string retval{};
        for (uint32_t row{0}; row < HEIGHT; ++row) {
            retval.append(get_line(row) + "\n");
        }
        return retval;
    }
};

} // namespace

void aoc::run() {
    std::ifstream instream{aoc::file::day_stream(4)};
    WordSearch    wordsearch{instream};
    WordSearch    dots{};

    aoc::print("DOTS:");
    aoc::print(std::string(dots));
}
