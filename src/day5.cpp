#include "util.h"
#include <vector>

namespace {
class Ordering {
    uint8_t m_first;
    uint8_t m_second;

public:
    explicit Ordering(const std::string& line)
        : m_first(aoc::string::char_to_uint(line[0], line[1]))
        , m_second(aoc::string::char_to_uint(line[3], line[4])) {}

    explicit operator std::string() const {
        return std::format("{}|{}", m_first, m_second);
    }

    [[nodiscard]]
    const uint8_t& first() const {
        return m_first;
    }

    [[nodiscard]]
    const uint8_t& second() const {
        return m_second;
    }
};

class Update {
    std::vector<uint8_t> m_buffer;

public:
    explicit Update(const std::string& line) {
        std::stringstream lineStream(line);
        std::string       cell;
        while (std::getline(lineStream, cell, ',')) {
            m_buffer.push_back(aoc::string::char_to_uint(cell[0], cell[1]));
        }
    }

    explicit operator std::string() const {
        std::string retval{};
        for (uint8_t item: m_buffer) {
            retval.append(std::to_string(item));
            retval.push_back(',');
        }
        return retval;
    }

    bool satisfies(const Ordering& ord) {
        const size_t pos_first{};
        const size_t pos_second{};
        if () {}
    }

    [[nodiscard]]
    const uint8_t& operator[](size_t idx) const {
        return m_buffer[idx];
    }
};

} // namespace

void aoc::run() {
    // NOLINTNEXTLINE(*-magic-numbers)
    std::ifstream         instream{aoc::file::day_stream(5)};
    std::vector<Ordering> orders{};
    std::vector<Update>   updates{};
    std::string           line;
    while (std::getline(instream, line)) {
        if (line.find('|') < line.length()) {
            orders.emplace_back(line);
        } else if (line.find(',') < line.length()) {
            updates.emplace_back(line);
        }
    }
    for (const auto& order: orders) {
        aoc::print(std::string(order));
    }
    for (const auto& update: updates) {
        aoc::print(std::string(update));
    }
}
