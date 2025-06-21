#include "util.h"
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <string>

namespace {

enum Direction : uint8_t {
    //     NESW
    N  = 0b1000,
    NE = 0b1100,
    E  = 0b0100,
    SE = 0b0110,
    S  = 0b0010,
    SW = 0b0011,
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

bool is_xmas(const std::string& test) {
    return (test == "XMAS");
}

template<size_t Len>
class Square {
    std::array<char, Len * Len> m_buffer;

public:
    explicit Square(std::array<char, Len * Len> buf)
        : m_buffer(std::move(buf)) {}

    const char& get(size_t row, size_t col) {
        // NOLINTNEXTLINE(*-constant-array-index)
        return m_buffer[(row * Len) + col];
    }

    bool mas(Direction dir) {
        // clang-format off
        switch (dir) {
        case NE: return ((get(2, 0) == 'M' && get(0, 2) == 'S'));
        case SE: return ((get(0, 0) == 'M' && get(2, 2) == 'S'));
        case SW: return ((get(0, 2) == 'M' && get(2, 0) == 'S'));
        case NW: return ((get(2, 2) == 'M' && get(0, 0) == 'S'));
		case N: case S: case E: case W:
			throw std::logic_error("bad dir");
        }
        // clang-format on
    }

    explicit operator std::string() {
        std::string retval{};
        retval.push_back(get(0, 0));
        retval.push_back(get(0, 1));
        retval.push_back(get(0, 2));
        retval.push_back('\n');
        retval.push_back(get(1, 0));
        retval.push_back(get(1, 1));
        retval.push_back(get(1, 2));
        retval.push_back('\n');
        retval.push_back(get(2, 0));
        retval.push_back(get(2, 1));
        retval.push_back(get(2, 2));
        return retval;
    }
};

class WordSearch {
private:
    static constexpr size_t AXIS_SIZE{140}; // 10 for test, 140 for full
    static constexpr size_t XMAS_LEN{4};
    static constexpr size_t m_width{AXIS_SIZE};
    static constexpr size_t m_height{AXIS_SIZE};
    std::array<char, m_height * m_width> m_buffer{};

    [[nodiscard]]
    static size_t idx(size_t row, size_t col) {
        if (row >= m_height) {
            throw std::runtime_error(
                std::format("row {} out of bounds, max: {}", row, m_height)
            );
        }
        if (col >= m_width) {
            throw std::runtime_error(
                std::format("col {} out of bounds, max: {}", col, m_width)
            );
        }
        return (row * m_width) + col;
    }

    char& get_mut(size_t row, size_t col) {
        return m_buffer[idx(row, col)]; // NOLINT
    }

public:
    explicit WordSearch(std::ifstream& infile) {
        std::string line;
        for (size_t row{0}; row < m_height; ++row) {
            std::getline(infile, line);
            aoc::debug(line);
            for (size_t col{0}; col < m_width; ++col) {
                get_mut(row, col) = line[col];
            }
        }
    }

    [[nodiscard]]
    const char& get_item(size_t row, size_t col) const {
        return m_buffer[idx(row, col)]; // NOLINT
    }

    static constexpr size_t SQUARE_SIZE{9};

    [[nodiscard]]
    std::array<char, SQUARE_SIZE> get_square(size_t row, size_t col) const {
        std::array<char, SQUARE_SIZE> retval{};
        // NOLINTBEGIN(*-magic-numbers)
        retval[0] = get_item(row - 1, col - 1);
        retval[1] = get_item(row - 1, col + 0);
        retval[2] = get_item(row - 1, col + 1);
        retval[3] = get_item(row + 0, col - 1);
        retval[4] = get_item(row + 0, col + 0);
        retval[5] = get_item(row + 0, col + 1);
        retval[6] = get_item(row + 1, col - 1);
        retval[7] = get_item(row + 1, col + 0);
        retval[8] = get_item(row + 1, col + 1);
        // NOLINTEND(*-magic-numbers)
        return retval;
    }

    [[nodiscard]]
    std::string get_string(size_t row, size_t col, Direction dir) const {
        std::string retval{};
        for (size_t mod{0}; mod < XMAS_LEN; ++mod) {
            size_t this_row{row};
            size_t this_col{col};
            if (north(dir)) {
                this_row -= mod;
            }
            if (south(dir)) {
                this_row += mod;
            }
            if (east(dir)) {
                this_col += mod;
            }
            if (west(dir)) {
                this_col -= mod;
            }
            try {
                retval.push_back(get_item(this_row, this_col));
            } catch (std::runtime_error&) {
                return {};
            }
        }
        return retval;
    }

    static size_t width() {
        return m_width;
    }

    static size_t height() {
        return m_height;
    }
};

} // namespace

void aoc::run() { // NOLINT(*-cognitive-complexity)
    std::ifstream    instream{aoc::file::day_stream()};
    const WordSearch wordsearch{instream};

    for (size_t row{0}; row < WordSearch::height(); ++row) {
        for (size_t col{0}; col < WordSearch::width(); ++col) {
            for (const Direction dir: DIRECTIONS) {
                const std::string tmp{wordsearch.get_string(row, col, dir)};
                if (is_xmas(tmp)) {
                    aoc::part1 += 1;
                }
            }
        }
    }
    for (size_t row{1}; row < WordSearch::height() - 1; ++row) {
        for (size_t col{1}; col < WordSearch::width() - 1; ++col) {
            if (wordsearch.get_item(row, col) == 'A') {
                Square<3>  sqr{wordsearch.get_square(row, col)};
                const bool is_mas{
                    false // NOLINT(*-simplify-boolean-expr)
                    || (sqr.mas(NE) && (sqr.mas(SE) || sqr.mas(NW)))
                    || (sqr.mas(NW) && (sqr.mas(SW) || sqr.mas(NE)))
                    || (sqr.mas(SE) && (sqr.mas(NE) || sqr.mas(SW)))
                    || (sqr.mas(SW) && (sqr.mas(NW) || sqr.mas(SE)))
                };
                aoc::debug("\nis_mas: {}", is_mas);
                aoc::debug(std::string(sqr));
                if (is_mas) {
                    aoc::part2 += 1;
                }
            }
        }
    }
}
