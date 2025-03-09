#include "util.h"

namespace {
constexpr size_t AXIS_SIZE{50}; // 12 for test, 50 for full

class Antenna {
    aoc::util::Coord m_pos;
    char             m_freq;

public:
    Antenna(size_t row, size_t col, char freq)
        : m_pos(row, col)
        , m_freq(freq) {}

    [[nodiscard]]
    const aoc::util::Coord& pos() const {
        return m_pos;
    }

    [[nodiscard]]
    const char& freq() const {
        return m_freq;
    }

    explicit operator std::string() const {
        return std::format("[{}: {}]", std::string(m_pos), m_freq);
    }
};

class Roof: public aoc::util::Grid<AXIS_SIZE, AXIS_SIZE> {
    using Grid = aoc::util::Grid<AXIS_SIZE, AXIS_SIZE>;

public:
    explicit Roof(std::ifstream& infile)
        : Grid(infile) {}

    void anti(aoc::util::Coord pos) {
        get_mut(pos.row(), pos.col()) = '#';
    }

    [[nodiscard]]
    std::vector<Antenna> find_antennae() const {
        std::vector<Antenna> antennae{};
        for (size_t row{0}; row < Roof::height(); ++row) {
            for (size_t col{0}; col < Roof::width(); ++col) {
                const char cha = get_item(row, col);
                if (cha != '.') {
                    antennae.emplace_back(row, col, cha);
                }
            }
        }
        return antennae;
    }
};

void check_antinode(
    std::vector<aoc::util::Coord>& antinodes,
    const Antenna&                 first_antenna,
    const Antenna&                 second_antenna,
    Roof&                          roof
) {
    const aoc::util::Coord anti =
        first_antenna.pos() + (first_antenna.pos() - second_antenna.pos());
    if ((first_antenna.freq() == second_antenna.freq())
        && (first_antenna.pos() != second_antenna.pos()) && Roof::check(anti)
        && (!aoc::in(anti, antinodes))) {
        aoc::print(
            "{}: {}, {}",
            std::string(anti),
            std::string(first_antenna),
            std::string(second_antenna)
        );
        antinodes.emplace_back(anti);
        roof.anti(anti);
    }
}

std::vector<aoc::util::Coord>
make_antinodes(const std::vector<Antenna>& antennae, Roof& roof) {
    std::vector<aoc::util::Coord> antinodes{};
    for (const Antenna& ant1: antennae) {
        for (const Antenna& ant2: antennae) {
            check_antinode(antinodes, ant1, ant2, roof);
            check_antinode(antinodes, ant2, ant1, roof);
        }
    }
    return antinodes;
}

void check_all_antinodes(
    std::vector<aoc::util::Coord>& antinodes,
    const Antenna&                 first_antenna,
    const Antenna&                 second_antenna,
    Roof&                          roof
) {
    const aoc::util::Coord mult = (first_antenna.pos() - second_antenna.pos());
    aoc::util::Coord       anti = first_antenna.pos() + mult;
    while (true) {
        if (!((first_antenna.freq() == second_antenna.freq())
              && (first_antenna.pos() != second_antenna.pos())
              && Roof::check(anti) && (!aoc::in(anti, antinodes)))) {
            return;
        }
        aoc::print(
            "{}: {}, {}",
            std::string(anti),
            std::string(first_antenna),
            std::string(second_antenna)
        );
        antinodes.emplace_back(anti);
        roof.anti(anti);
        anti += mult;
    }
}

std::vector<aoc::util::Coord>
make_all_antinodes(const std::vector<Antenna>& antennae, Roof& roof) {
    std::vector<aoc::util::Coord> antinodes{};
    for (const Antenna& ant1: antennae) {
        for (const Antenna& ant2: antennae) {
            check_all_antinodes(antinodes, ant1, ant2, roof);
            check_all_antinodes(antinodes, ant2, ant1, roof);
        }
    }
    return antinodes;
}

void run_part_1(Roof roof) {
    const std::vector<Antenna>          antennae{roof.find_antennae()};
    const std::vector<aoc::util::Coord> antinodes{make_antinodes(antennae, roof)
    };
    aoc::part1 = antinodes.size();
}

void run_part_2(Roof roof) {
    const std::vector<Antenna>          antennae{roof.find_antennae()};
    const std::vector<aoc::util::Coord> antinodes{
        make_all_antinodes(antennae, roof)
    };
    aoc::part2 = antinodes.size();
    aoc::print(std::string(roof));
}

} // namespace

void aoc::run() {
    std::ifstream instream{aoc::file::day_stream()};
    const Roof    roof{instream};
    aoc::print(std::string(roof));
    run_part_1(roof);
    run_part_2(roof);
}
