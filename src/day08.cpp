#include "util.h"

namespace {
using aoc::util::Coord;

class Antenna {
    Coord m_pos;
    char  m_freq;

public:
    Antenna(size_t row, size_t col, char freq)
        : m_pos(row, col)
        , m_freq(freq) {}

    [[nodiscard]]
    const Coord& pos() const {
        return m_pos;
    }

    [[nodiscard]]
    const char& freq() const {
        return m_freq;
    }

    bool operator!=(const Antenna& other) const {
        return (m_pos != other.m_pos);
    }

    explicit operator std::string() const {
        return std::format("[{}: {}]", std::string(m_pos), m_freq);
    }
};

class Roof: public aoc::util::Grid {
public:
    explicit Roof(std::ifstream& infile)
        : Grid(infile, aoc::test_mode ? 12U : 50U) {} // NOLINT(*-magic-numbers)

    void anti(Coord pos) {
        (*this)[pos.x(), pos.y()] = '#';
    }

    [[nodiscard]]
    std::vector<Antenna> find_antennae() const {
        std::vector<Antenna> antennae{};
        for (size_t ypos{0}; ypos < height(); ++ypos) {
            for (size_t xpos{0}; xpos < width(); ++xpos) {
                const char cha = (*this)[xpos, ypos];
                if (cha != '.') {
                    antennae.emplace_back(xpos, ypos, cha);
                }
            }
        }
        return antennae;
    }
};

void check_antinode(
    std::vector<Coord>& antinodes,
    const Antenna&      first_antenna,
    const Antenna&      second_antenna,
    Roof&               roof
) {
    const Coord anti =
        first_antenna.pos() + (first_antenna.pos() - second_antenna.pos());
    if (roof.check(anti) && !aoc::in(anti, antinodes)) {
        antinodes.emplace_back(anti);
        roof.anti(anti);
    }
}

void check_all_antinodes(
    std::vector<Coord>& antinodes,
    const Antenna&      first_antenna,
    const Antenna&      second_antenna,
    Roof&               roof
) {
    const Coord move = (first_antenna.pos() - second_antenna.pos());
    Coord       anti = first_antenna.pos();
    while (roof.check(anti)) {
        if (!aoc::in(anti, antinodes)) {
            aoc::debug(
                "{}: {} - {}, {}",
                second_antenna.freq(),
                std::string(anti),
                std::string(first_antenna.pos()),
                std::string(second_antenna.pos())
            );
            antinodes.emplace_back(anti);
            roof.anti(anti);
        }
        anti += move;
    }
}

void run_part_1(Roof roof) {
    const std::vector<Antenna> antennae{roof.find_antennae()};
    std::vector<Coord>         antinodes{};
    for (const Antenna& ant1: antennae) {
        for (const Antenna& ant2: antennae) {
            if (ant1.freq() == ant2.freq() && ant1 != ant2) {
                check_antinode(antinodes, ant1, ant2, roof);
                check_antinode(antinodes, ant2, ant1, roof);
            }
        }
    }
    aoc::part1 = antinodes.size();
}

void run_part_2(Roof roof) {
    const std::vector<Antenna> antennae{roof.find_antennae()};
    std::vector<Coord>         antinodes{};
    for (const Antenna& antenna: antennae) {
        aoc::debug(std::string(antenna));
    }
    for (const Antenna& ant1: antennae) {
        for (const Antenna& ant2: antennae) {
            if (ant1.freq() == ant2.freq() && ant1 != ant2) {
                check_all_antinodes(antinodes, ant1, ant2, roof);
                check_all_antinodes(antinodes, ant2, ant1, roof);
            }
        }
    }
    aoc::part2 = antinodes.size();
    aoc::debug(std::string(roof));
}

} // namespace

void aoc::run() {
    std::ifstream instream{aoc::file::day_stream()};
    const Roof    roof{instream};
    aoc::debug(std::string(roof));
    run_part_1(roof);
    run_part_2(roof);
}
