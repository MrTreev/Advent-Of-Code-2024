#include "util.h"
#include <mdspan>
#include <ranges>
#include <stdexcept>

namespace {
constexpr size_t AXIS_SIZE{10}; // 10 for test, 130 for full

using Coordinate = aoc::util::Coordinate<size_t>;
using aoc::util::Direction;

[[nodiscard]]
bool in_range(Coordinate pos) {
    return ((pos.x() < AXIS_SIZE) && (pos.y() < AXIS_SIZE));
}

class Map {
private:
    static constexpr size_t                                WIDTH{AXIS_SIZE};
    static constexpr size_t                                HEIGHT{AXIS_SIZE};
    std::array<char, (WIDTH * HEIGHT)>                     m_buffer{0};
    std::mdspan<char, std::extents<size_t, WIDTH, HEIGHT>> m_mdspan;

    auto&& get_item(this auto&& self, const size_t& xpos, const size_t& ypos) {
        return self.m_mdspan[ypos, xpos];
    }

public:
    explicit Map(std::ifstream& infile)
        : m_buffer({})
        , m_mdspan(m_buffer.data(), WIDTH, HEIGHT) {
        std::string line;
        for (size_t row{0}; row < height(); ++row) {
            std::getline(infile, line);
            for (size_t col{0}; col < width(); ++col) {
                get_item(row, col) = line[col];
            }
        }
    }

    Map(const Map& other)
        : m_buffer(other.m_buffer)
        , m_mdspan(m_buffer.data(), WIDTH, HEIGHT) {}

    Map(Map&& other)           = delete;
    Map& operator=(const Map&) = default;
    Map& operator=(Map&&)      = delete;

    Map(const Map& other, size_t idx)
        : Map(other) {
        block(idx);
    }

    void block(size_t idx) {
        m_buffer[idx] = '#'; // NOLINT(*-constant-array-index)
    }

    auto&& operator[](this auto&& self, size_t xpos, size_t ypos) {
        return self.get_item(xpos, ypos);
    }

    auto&& operator[](this auto&& self, Coordinate pos) {
        return self.get_item(pos.x(), pos.y());
    }

    explicit operator std::string() const {
        std::string outstr{};
        for (size_t row{0}; row < height(); ++row) {
            for (size_t col{0}; col < width(); ++col) {
                const char item = get_item(row, col);
                outstr.push_back(item);
            }
            outstr.push_back('\n');
        }
        return outstr;
    }

    [[nodiscard]]
    static constexpr size_t width() {
        return WIDTH;
    }

    [[nodiscard]]
    static constexpr size_t height() {
        return HEIGHT;
    }

    [[nodiscard]]
    static constexpr size_t elements() {
        return WIDTH * HEIGHT;
    }
};

class Guard {
    Direction  m_facing{Direction::N};
    Coordinate m_position{0, 0};
    Coordinate m_initial{0, 0};

public:
    explicit Guard(const Map& map) {
        for (size_t row{0}; row < Map::height(); ++row) {
            for (size_t col{0}; col < Map::width(); ++col) {
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
    bool check_next() const {
        const Coordinate next_pos = (m_position + m_facing);
        return in_range(next_pos);
    }

    [[nodiscard]]
    const Coordinate& position() const {
        return m_position;
    }
};

void run_part_1(const Map& map, Guard guard) {
    std::vector<Coordinate> coords{};
    coords.push_back(guard.position());
    while (guard.check_next()) {
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
    for (const size_t idx: std::views::iota(0ULL, Map::elements())) {
        std::vector<Guard> values{};
        const Map          lmap{map, idx};
        aoc::debug("Running\n{}", std::string(lmap));
        while (guard.check_next()) {
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
