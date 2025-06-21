#include "util.h"
#include <ranges>
#include <stdexcept>

namespace {

using aoc::util::Coord;
using aoc::util::Direction;

class Map: public aoc::util::Grid {
public:
    explicit Map(std::ifstream& infile)
        : Grid(infile, aoc::test_mode ? 10U : 130U) // NOLINT(*-magic-numbers)
    {}

    Map(const Map& other, size_t idx)
        : Map(other) {
        block(idx % width(), idx / height());
    }

    void block(size_t xpos, size_t ypos) {
        (*this)[xpos, ypos] = '#'; // NOLINT(*-constant-array-index)
    }

    [[nodiscard]]
    bool in_range(Coord pos) const {
        return ((pos.x() < width()) && (pos.y() < height()));
    }
};

class Guard {
    Direction m_facing{Direction::N};
    Coord     m_position{0, 0};
    Coord     m_initial{0, 0};

public:
    explicit Guard(const Map& map) {
        for (size_t row{0}; row < map.height(); ++row) {
            for (size_t col{0}; col < map.width(); ++col) {
                if (map[row, col] == '^') {
                    m_position = {row, col};
                    m_initial  = {row, col};
                    return;
                }
            }
        }
        throw std::runtime_error("AAAA");
    }

    void reset() {
        m_position = m_initial;
        m_facing   = Direction::N;
    }

    void turn() {
        switch (m_facing) {
        case Direction::N: m_facing = Direction::E; return;
        case Direction::E: m_facing = Direction::S; return;
        case Direction::S: m_facing = Direction::W; return;
        case Direction::W: m_facing = Direction::N; return;
        }
    }

    bool operator==(const Guard&) const = default;

    void next(const Map& map) {
        if (map[m_position + m_facing] == '#') {
            turn();
        } else {
            m_position = (m_position + m_facing);
        }
    }

    [[nodiscard]]
    bool check_next(const Map& map) const {
        const Coord next_pos = (m_position + m_facing);
        return map.in_range(next_pos);
    }

    [[nodiscard]]
    const Coord& position() const {
        return m_position;
    }
};

void run_part_1(const Map& map, Guard guard) {
    std::vector<Coord> coords{};
    coords.push_back(guard.position());
    while (guard.check_next(map)) {
        guard.next(map);
        if (!aoc::in(guard.position(), coords)) {
            coords.push_back(guard.position());
        }
    }
    aoc::part1 = coords.size();
}

void run_part_2(const Map& map, const Guard& inguard) {
    Guard guard{inguard};
    aoc::debug("");
    size_t count{0};
    for (const size_t idx: std::views::iota(0ULL, map.elements())) {
        std::vector<Guard> values{};
        const Map          lmap{map, idx};
        aoc::debug("Running\n{}", std::string(lmap));
        while (guard.check_next(map)) {
            guard.next(lmap);
            if (aoc::in(guard, values)) {
                aoc::debug("found loop at {}", idx);
                count++;
                break;
            }
            values.emplace_back(guard);
        }
        guard.reset();
        aoc::debug("found no loop at {}", idx);
    }
    aoc::part2 = count;
}

} // namespace

void aoc::run() {
    std::ifstream instream{aoc::file::day_stream()};
    const Map     map{instream};
    const Guard   guard{map};
    debug(std::string(map));
    run_part_1(map, guard);
    run_part_2(map, guard);
}
