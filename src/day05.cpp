#include "util.h"
#include <cassert>
#include <stdexcept>
#include <utility>
#include <vector>

namespace {
using pair = std::pair<uint8_t, uint8_t>;

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
        std::string retval;
        for (uint8_t item: m_buffer) {
            retval.append(std::format("{:>2},", item));
        }
        return retval;
    }

    [[nodiscard]]
    bool satisfies(const pair& ord) const {
        size_t pos_first{0};
        size_t pos_second{0};
        for (size_t idx{0}; idx < m_buffer.size(); ++idx) {
            if (m_buffer[idx] == ord.first) {
                pos_first = idx;
            }
            if (m_buffer[idx] == ord.second) {
                pos_second = idx;
            }
            if ((pos_first != 0) && (pos_second != 0)) {
                if (pos_first >= pos_second) {
                    return false;
                }
            }
        }
        return true;
    }

    [[nodiscard]]
    const uint8_t& mid() const {
        return m_buffer[m_buffer.size() / 2];
    }

    [[nodiscard]]
    size_t find(uint8_t f_item, size_t start = 0) const {
        for (size_t idx{start}; idx < m_buffer.size(); ++idx) {
            if (m_buffer[idx] == f_item) {
                return idx;
            }
        }
        throw std::runtime_error(std::format("no item '{}' found", f_item));
    }

    void reorder(const pair& ord) {
        const size_t idx_second = find(ord.second);
        const size_t idx_first  = find(ord.first, idx_second);
        for (size_t idx{idx_second}; idx < idx_first; ++idx) {
            const uint8_t tmp_item = m_buffer[idx];
            m_buffer[idx]          = m_buffer[idx + 1];
            m_buffer[idx + 1]      = tmp_item;
        }
    }
};

bool all_satisfies(const Update& update, const std::vector<pair>& orders) {
    aoc::debug(std::string(update));
    for (const auto& order: orders) {
        if (!update.satisfies(order)) {
            const auto [first, second] = order;
            aoc::debug("{}|{}", first, second);
            return false;
        }
    }
    return true;
}

std::pair<std::vector<pair>, std::vector<Update>>
make_vecs(std::ifstream& instream) {
    std::vector<pair>   orders{};
    std::vector<Update> updates{};
    for (std::string line; std::getline(instream, line);) {
        if (line.find('|') < line.length()) {
            orders.emplace_back(
                aoc::string::char_to_uint(line[0], line[1]),
                aoc::string::char_to_uint(line[3], line[4])
            );
        } else if (line.find(',') < line.length()) {
            updates.emplace_back(line);
        }
    }
    return std::make_pair(orders, updates);
}

} // namespace

void aoc::run() {
    std::ifstream instream{aoc::file::day_stream()};
    const auto [orders, updates] = make_vecs(instream);

    for (const auto& order: orders) {
        const auto [first, second] = order;
        aoc::debug("{}|{}", first, second);
    }
    aoc::debug("Part 1");
    std::vector<Update> bad_updates{};
    for (const auto& update: updates) {
        aoc::print("");
        if (all_satisfies(update, orders)) {
            aoc::part1 += update.mid();
        } else {
            bad_updates.emplace_back(update);
        }
        aoc::print("");
    }

    aoc::debug("Part 2");
    for (auto& update: bad_updates) {
        aoc::print("");
        while (!all_satisfies(update, orders)) {
            for (const auto& order: orders) {
                if (!update.satisfies(order)) {
                    update.reorder(order);
                }
            }
        }
        aoc::part2 += update.mid();
        aoc::print("");
    }
}
