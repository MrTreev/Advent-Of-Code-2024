#include "util.h"
#include <execution>
#include <stdexcept>

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
    bool in_range() const {
        return ((m_row < AXIS_SIZE) && (m_col < AXIS_SIZE));
    }

    [[nodiscard]]
    size_t x() const {
        return m_row;
    }

    [[nodiscard]]
    size_t y() const {
        return m_col;
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-member-function"
    bool operator==(const Coordinate& other) const = default;
#pragma clang diagnostic pop
};

class Map {
private:
    static constexpr size_t          WIDTH{AXIS_SIZE};
    static constexpr size_t          HEIGHT{AXIS_SIZE};
    std::array<char, HEIGHT * WIDTH> m_buffer{0};

    static size_t idx(size_t row, size_t col) {
        if (row >= height()) {
            throw std::runtime_error(
                std::format("row {} out of bounds, max: {}", row, height())
            );
        }
        if (col >= width()) {
            throw std::runtime_error(
                std::format("col {} out of bounds, max: {}", col, width())
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
        for (size_t row{0}; row < height(); ++row) {
            std::getline(infile, line);
            for (size_t col{0}; col < width(); ++col) {
                get_mut(row, col) = line[col];
            }
        }
    }

    Map(const Map& other, size_t idx)
        : Map(other) {
        block(idx);
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

    static constexpr size_t width() {
        return WIDTH;
    }

    static constexpr size_t height() {
        return HEIGHT;
    }

    explicit operator std::string() const {
        std::string outstr{};
        for (size_t row{0}; row < height(); ++row) {
            for (size_t col{0}; col < width(); ++col) {
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

    void next(const Map& map) {
        if (map.get_item(m_position + m_facing) == '#') {
            turn();
        } else {
            m_position = (m_position + m_facing);
        }
    }

    [[nodiscard]]
    bool check_next() const {
        const Coordinate next_pos = (m_position + m_facing);
        return next_pos.in_range();
    }

    [[nodiscard]]
    const Coordinate& position() const {
        return m_position;
    }

    [[nodiscard]]
    explicit operator std::string() const {
        return std::format(
            "{}: {}",
            std::string(m_position),
            to_string(m_facing)
        );
    }
};

void run_part_1(Map map, const Guard& inguard) {
    std::vector<Coordinate> coords{};
    Guard                   guard{inguard};
    coords.push_back(guard.position());

    while (guard.check_next()) {
        guard.next(map);
        if (!aoc::in(guard.position(), coords)) {
            coords.push_back(guard.position());
        }
    }
    aoc::part1 = coords.size();
}

bool run_iter(Guard guard, Map inmap, size_t idx) {
    std::vector<std::string> items{};
    Map                      map{inmap, idx};
    while (guard.check_next()) {
        guard.next(map);
        const std::string g_item{guard};
        if (aoc::in(g_item, items)) {
            aoc::debug("found loop at {}", idx);
            return true;
        }
        items.emplace_back(g_item);
    }
    aoc::debug("found no loop at {}", idx);
    return false;
}

void run_part_2(Map map, const Guard& inguard) {
    const Guard guard{inguard};
    aoc::debug("");
    std::vector<size_t> indexs(map.size());
    std::vector<bool>   items(map.size());
    std::iota(indexs.begin(), indexs.end(), 0);
    std::for_each(
        std::execution::par_unseq,
        indexs.begin(),
        indexs.end(),
        [&](size_t idx) { items[idx] = run_iter(guard, map, idx); }
    );
    aoc::part2 =
        static_cast<size_t>(std::count(items.begin(), items.end(), true));
}

} // namespace

void aoc::run() {
    std::ifstream instream{aoc::file::day_stream(6)}; // NOLINT(*-magic-numbers)
    const Map     map{instream};
    const Guard   guard{map};
    print(std::string(map));
    run_part_1(map, guard);
    assert(aoc::part1 == 5145UL);
    run_part_2(map, guard);
    assert(aoc::part1 == 1523UL);
}
