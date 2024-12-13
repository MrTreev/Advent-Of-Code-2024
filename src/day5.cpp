#include "util.h"
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <vector>

namespace {
class Ordering {
    uint8_t m_first;
    uint8_t m_second;

public:
    explicit Ordering(const std::string& line)
        : m_first(aoc::string::char_to_uint(line[0], line[1]))
        , m_second(aoc::string::char_to_uint(line[3], line[4])) {}

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
    using item_t = uint8_t;
    std::vector<item_t> m_buffer;

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
        for (item_t item: m_buffer) {
            retval.append(std::format("{:>2},", item));
        }
        return retval;
    }

    [[nodiscard]]
    bool satisfies(const Ordering& ord) const {
        size_t pos_first{0};
        size_t pos_second{0};
        for (size_t idx{0}; idx < m_buffer.size(); ++idx) {
            if (m_buffer[idx] == ord.first()) {
                pos_first = idx;
            }
            if (m_buffer[idx] == ord.second()) {
                pos_second = idx;
            }
            if ((pos_first != 0) && (pos_second != 0)) {
                return (pos_first < pos_second);
            }
        }
        return true;
    }

    [[nodiscard]]
    const item_t& mid() const {
        return m_buffer[m_buffer.size() / 2];
    }

    size_t find(item_t f_item, size_t start = 0) {
        for (size_t idx{start}; idx < m_buffer.size(); ++idx) {
            if (m_buffer[idx] == f_item) {
                return idx;
            }
        }
        throw std::runtime_error(std::format("no item '{}' found", f_item));
    }

    void reorder(const Ordering& ord) {
        size_t idx_second = find(ord.second());
        size_t idx_first  = find(ord.first(), idx_second);
        for (size_t idx{idx_second}; idx < idx_first; ++idx) {
            const item_t tmp_item = m_buffer[idx];
            m_buffer[idx]         = m_buffer[idx + 1];
            m_buffer[idx + 1]     = tmp_item;
        }
    }
};

bool all_satisfies(const Update& update, const std::vector<Ordering>& orders) {
    return (std::all_of(
        orders.cbegin(),
        orders.cend(),
        [update](const Ordering& ooo) { return update.satisfies(ooo); }
    ));
}

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

    std::vector<Update> bad_updates{};

    for (const auto& update: updates) {
        if (all_satisfies(update, orders)) {
            aoc::debug("good: {}", std::string(update));
            aoc::debug("mid:  {}", std::to_string(update.mid()));
            aoc::part1 += update.mid();
        } else {
            bad_updates.emplace_back(update);
        }
    }
    assert(aoc::part1 == 5091);

    for (auto& update: bad_updates) {
        while (!all_satisfies(update, orders)) {
            for (const auto& order: orders) {
                if (!update.satisfies(order)) {
                    update.reorder(order);
                }
            }
        }
        aoc::part2 += update.mid();
        aoc::debug("sort: {}", std::string(update));
        aoc::debug("mid:  {}", std::to_string(update.mid()));
    }
    //assert(aoc::part2 == );
}
