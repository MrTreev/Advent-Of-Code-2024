#include "util.h"
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {
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

// std::string to_string(Direction dir) {
//     switch (dir) {
//     case N:
//         return "N ";
//     case NE:
//         return "NE";
//     case E:
//         return "E ";
//     case SE:
//         return "SE";
//     case S:
//         return "S ";
//     case SW:
//         return "SW";
//     case W:
//         return "W ";
//     case NW:
//         return "NW";
//     }
// }
//
// constexpr std::array<Direction, 8> DIRECTIONS{
//     {
//      N, NE,
//      E, SE,
//      S, SW,
//      W, NW,
//      }
// };
//
// bool north(Direction dir) {
//     return (dir & Direction::N) != 0;
// }
//
// bool south(Direction dir) {
//     return (dir & Direction::S) != 0;
// }
//
// bool east(Direction dir) {
//     return (dir & Direction::E) != 0;
// }
//
// bool west(Direction dir) {
//     return (dir & Direction::W) != 0;
// }

class WordSearch {
private:
    static constexpr size_t FULL_SIZE{140};
    static constexpr size_t TEST_SIZE{10};

    std::vector<char> m_buffer;
    size_t            m_width{FULL_SIZE};
    size_t            m_height{FULL_SIZE};

    void set_sizes() {
        if (aoc::test_mode) {
            m_height = TEST_SIZE;
            m_width  = TEST_SIZE;
        }
    }

    [[nodiscard]]
    size_t idx(size_t row, size_t col) const {
        if (std::cmp_greater(row, m_height)) {
            throw std::runtime_error(
                std::format("row {} out of bounds, max: {}", row, m_height)
            );
        }
        if (std::cmp_greater(col, m_width)) {
            throw std::runtime_error(
                std::format("col {} out of bounds, max: {}", col, m_width)
            );
        }
        return (row * m_width) + col;
    }

    [[nodiscard]]
    std::string get_line(size_t row) const {
        return {&m_buffer[idx(row, 0)], m_width};
    }

public:
    explicit WordSearch(std::ifstream& infile) {
        set_sizes();
        m_buffer.resize(m_width * m_height);
        std::string line;
        for (size_t row{0}; row < m_height; ++row) {
            std::getline(infile, line);
            for (size_t col{0}; col < m_width; ++col) {
                m_buffer[idx(row, col)] = line[col];
            }
        }
    }

    explicit operator std::string() {
        std::string retval{};
        for (uint32_t row{0}; row < m_height; ++row) {
            retval.append(get_line(row) + "\n");
        }
        return retval;
    }
};

} // namespace

void aoc::run() {
    std::ifstream instream{aoc::file::day_stream(4)};
    WordSearch    wordsearch{instream};
    WordSearch    dots{wordsearch};

    aoc::print("DOTS:");
    aoc::print(std::string(dots));
}
