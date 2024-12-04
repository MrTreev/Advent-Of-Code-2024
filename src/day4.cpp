#include "util.h"
#include <cassert>
#include <cstddef>
#include <string>
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

constexpr std::array<Direction, 8> DIRECTIONS{
    {
     N, NE,
     E, SE,
     S, SW,
     W, NW,
     }
};

class WordSearch {
private:
    std::array<char, WIDTH * HEIGHT> m_buffer{};

    static size_t idx(size_t row, size_t col) {
        return (row * WIDTH) + col;
    }

    char get_item(const size_t row, const size_t col) {
        // NOLINTBEGIN(cppcoreguidelines-pro-bounds-constant-array-index)
        return m_buffer[idx(row, col)];
        // NOLINTEND(cppcoreguidelines-pro-bounds-constant-array-index)
    }

public:
    explicit WordSearch(std::ifstream& infile) {
        std::string line;
        size_t      lineno = 0;
        while (std::getline(infile, line)) {
            assert(line.size() == WIDTH);
            AOC_UBU(auto* bufiter = m_buffer.begin() + (lineno * WIDTH);)
            std::copy(line.cbegin(), line.cend(), bufiter);
            ++lineno;
            for (size_t idx{0}; idx < m_buffer.size(); ++idx) {
                // NOLINTBEGIN(cppcoreguidelines-pro-bounds-constant-array-index)
                if (idx < (lineno * WIDTH)) {
                    assert(m_buffer[idx] != 0);
                } else {
                    assert(m_buffer[idx] == 0);
                }
                // NOLINTEND(cppcoreguidelines-pro-bounds-constant-array-index)
            }
        }
    }

    char operator[](const size_t row, const size_t col) {
        return get_item(row, col);
    }

    std::string
    getstr(const size_t row, const size_t col, const Direction dir) {
        constexpr size_t LEN{3};
        std::string      retval;
        if (((dir & Direction::N) == dir) && (row < LEN)) {
            return retval;
        }
        if (((dir & Direction::S) == dir) && (row > (HEIGHT - LEN))) {
            return retval;
        }
        if (((dir & Direction::E) == dir) && (row > (WIDTH - LEN))) {
            return retval;
        }
        if (((dir & Direction::W) == dir) && (col < LEN)) {
            return retval;
        }
        const long modcol = ((dir & Direction::N) / Direction::N)
                            - ((dir & Direction::S) / Direction::S);
        const long modrow = ((dir & Direction::E) / Direction::E)
                            - ((dir & Direction::W) / Direction::W);
        retval.push_back(get_item(
            static_cast<size_t>(static_cast<long>(row) + (0L * modrow)),
            static_cast<size_t>(static_cast<long>(col) + (0L * modcol))
        ));
        retval.push_back(get_item(
            static_cast<size_t>(static_cast<long>(row) + (1L * modrow)),
            static_cast<size_t>(static_cast<long>(col) + (1L * modcol))
        ));
        retval.push_back(get_item(
            static_cast<size_t>(static_cast<long>(row) + (2L * modrow)),
            static_cast<size_t>(static_cast<long>(col) + (2L * modcol))
        ));
        retval.push_back(get_item(
            static_cast<size_t>(static_cast<long>(row) + (3L * modrow)),
            static_cast<size_t>(static_cast<long>(col) + (3L * modcol))
        ));
        return retval;
    }
};

} // namespace

void aoc::run() {
    std::ifstream instream{aoc::file::day_stream(4)};
    WordSearch    wordsearch{instream};
    WordSearch    dots{instream};
    for (size_t row{0}; row < WIDTH; ++row) {
        for (size_t col{0}; col < HEIGHT; ++col) {
            if (wordsearch[row, col] == 'X') {
                for (const Direction dir: DIRECTIONS) {
                    const std::string tmp_str{wordsearch.getstr(row, col, dir)};
                    if (tmp_str == "XMAS" || tmp_str == "SAMX") {
                        aoc::part1 += 1;
                    }
                }
            }
        }
    }
}
