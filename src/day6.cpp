#include "util.h"
#include <algorithm>
#include <cassert>
#include <vector>

namespace {
constexpr size_t AXIS_SIZE{130}; // 10 for test, 130 for full
enum class Direction : uint8_t { N, E, S, W };

std::string to_string(Direction dir) {
    switch (dir) {
    case Direction::N: return "N";
    case Direction::E: return "S";
    case Direction::S: return "E";
    case Direction::W: return "W";
    }
};

class Coordinate {
    size_t m_row;
    size_t m_col;

public:
    constexpr Coordinate(size_t row, size_t col)
        : m_row(row)
        , m_col(col) {}

    Coordinate operator+(const Direction& dir) const {
        switch (dir) {
        case Direction::N: return {m_row - 1, m_col};
        case Direction::E: return {m_row, m_col + 1};
        case Direction::S: return {m_row + 1, m_col};
        case Direction::W: return {m_row, m_col - 1};
        }
    }

    explicit operator std::string() const {
        return std::format("({}, {})", m_row, m_col);
    }

    [[nodiscard]]
    size_t x() const {
        return m_row;
    }

    [[nodiscard]]
    size_t y() const {
        return m_col;
    }

    bool operator==(const Coordinate& other) const = default;

    bool operator>(const Coordinate& other) const {
        return (m_row > other.m_row || m_col > other.m_col);
    }
};

class Map {
private:
    static constexpr size_t          WIDTH{AXIS_SIZE};
    static constexpr size_t          HEIGHT{AXIS_SIZE};
    std::array<char, HEIGHT * WIDTH> m_buffer{0};

    static size_t idx(size_t row, size_t col) {
        if (row >= HEIGHT) {
            throw std::runtime_error(
                std::format("row {} out of bounds, max: {}", row, HEIGHT)
            );
        }
        if (col >= WIDTH) {
            throw std::runtime_error(
                std::format("col {} out of bounds, max: {}", col, WIDTH)
            );
        }
        return (row * WIDTH) + col;
    }

    char& get_mut(size_t row, size_t col) {
        return m_buffer[idx(row, col)]; // NOLINT
    }

public:
    explicit Map(std::ifstream& infile) {
        std::string line;
        for (size_t row{0}; row < HEIGHT; ++row) {
            std::getline(infile, line);
            for (size_t col{0}; col < WIDTH; ++col) {
                get_mut(row, col) = line[col];
            }
        }
    }

    static Coordinate idx(size_t idx) {
        if (idx >= (WIDTH * HEIGHT)) {
            throw std::runtime_error(std::format(
                "idx {} out of bounds, max: {}",
                idx,
                (WIDTH * HEIGHT)
            ));
        }
        return {(idx / WIDTH), (idx % WIDTH)};
    }

    void block(const size_t idx) {
        m_buffer[idx] = '#'; // NOLINT(*-array-index)
    }

    [[nodiscard]]
    const char& get_item(size_t row, size_t col) const {
        return m_buffer[idx(row, col)]; // NOLINT
    }

    [[nodiscard]]
    constexpr size_t size() const {
        return m_buffer.size();
    }

    [[nodiscard]]
    const char& get_item(const Coordinate& coord) const {
        return m_buffer[idx(coord.x(), coord.y())]; // NOLINT
    }

    [[nodiscard]]
    auto begin() {
        return m_buffer.begin();
    }

    [[nodiscard]]
    auto end() {
        return m_buffer.end();
    }

    static constexpr size_t width() {
        return WIDTH;
    }

    static constexpr size_t height() {
        return HEIGHT;
    }

    explicit operator std::string() const {
        std::string outstr{};
        for (size_t row{0}; row < HEIGHT; ++row) {
            for (size_t col{0}; col < WIDTH; ++col) {
                char item = get_item(row, col);
                outstr.push_back(item);
            }
            outstr.push_back('\n');
        }
        return outstr;
    }
};

class Guard {
    Direction  m_facing{Direction::N};
    Coordinate m_position{0, 0};

public:
    explicit Guard(const Map& map) {
        for (size_t row{0}; row < Map::height(); ++row) {
            for (size_t col{0}; col < Map::width(); ++col) {
                if (map.get_item(row, col) == '^') {
                    m_position = {row, col};
                    return;
                }
            }
        }
        throw std::runtime_error("AAAA");
    }

    void turn() {
        switch (m_facing) {
        case Direction::N: m_facing = Direction::E; return;
        case Direction::E: m_facing = Direction::S; return;
        case Direction::S: m_facing = Direction::W; return;
        case Direction::W: m_facing = Direction::N; return;
        }
    }

    bool check_next() {
        constexpr Coordinate max_pos = {Map::width(), Map::height()};
        return !((m_position + m_facing) > max_pos);
    }

    void next(const Map& map) {
        if (map.get_item(m_position + m_facing) == '#') {
            turn();
        } else {
            m_position = (m_position + m_facing);
        }
    }

    const Coordinate& position() {
        return m_position;
    }

    explicit operator std::string() const {
        return std::format(
            "{}: {}",
            std::string(m_position),
            to_string(m_facing)
        );
    }
};

bool in(auto item, const std::vector<decltype(item)>& vec) {
    return std::ranges::any_of(vec.begin(), vec.end(), [item](auto cmp) {
        return cmp == item;
    });
}

void run_part_1(Map map, const Guard& inguard) {
    std::vector<Coordinate> coords{};
    Guard                   guard{inguard};
    coords.push_back(guard.position());

    while (guard.check_next()) {
        guard.next(map);
        if (!in(guard.position(), coords)) {
            coords.push_back(guard.position());
        }
    }
    aoc::part1 = coords.size();
    assert(aoc::part1 == 5145UL);
}

bool run_iter(Guard guard, Map map) {
    std::vector<std::string> items{};
    while (guard.check_next()) {
        guard.next(map);
        auto g_item = std::string(guard);
        if (in(g_item, items)) {
            return true;
        }
        items.emplace_back(g_item);
    }
    return false;
}

void run_part_2(Map map, const Guard& inguard) {
    const Guard guard{inguard};
    aoc::debug("");
    for (auto& pos: map) {
        if (pos != '#') {
            if (run_iter(guard, map)) {
                aoc::part2 += 1;
                aoc::debug("found loop");
            } else {
                aoc::debug("no loop");
            }
        }
    }
    aoc::debug("");
}

} // namespace

void aoc::run() {
    std::ifstream instream{aoc::file::day_stream(6)}; // NOLINT(*-magic-numbers)
    const Map     map{instream};
    const Guard   guard{map};
    print(std::string(map));
    run_part_1(map, guard);
    run_part_2(map, guard);
}
