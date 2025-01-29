#include "util.h"
#include <bitset>
#include <climits>
#include <cmath>
#include <string>

namespace {

class Operation {
    size_t              m_result;
    std::vector<size_t> m_items;

    [[nodiscard]]
    std::string vec2str() const {
        std::string retval;
        for (const auto& item: m_items) {
            retval.append(std::to_string(item) + " ");
        }
        return retval;
    }

public:
    explicit Operation(const std::string& line)
        : m_result(std::stoul({line, 0, line.find(':')})) {
        std::stringstream stuff{
            std::string(line, line.find(':') + 2, line.size())
        };
        for (std::string item; std::getline(stuff, item, ' ');) {
            m_items.push_back(std::stoul(item));
        }
    }

    explicit operator std::string() const {
        return std::format("{}: {}", m_result, vec2str());
    }

    [[nodiscard]]
    size_t result() const {
        return m_result;
    }

    [[nodiscard]]
    const std::vector<size_t>& items() const {
        return m_items;
    }
};

bool operate(const Operation& operation) {
    const std::vector<size_t>& items = operation.items();
    for (size_t mask{0}; mask < std::powl(2, items.size() - 1); ++mask) {
        std::bitset<32> bits{mask}; // NOLINT(*-magic-numbers)
        size_t          sum{items[0]};
        for (size_t idx{1}; idx < items.size(); ++idx) {
            std::string line;
            if (bits[idx - 1]) {
                sum *= items[idx];
            } else {
                sum += items[idx];
            }
            line.append(std::format(" = {}", sum));
        }
        aoc::debug("sum: {}", sum);
        if (operation.result() == sum) {
            return true;
        }
    }
    return false;
}

void run_part_1(const std::vector<Operation>& operations) {
    for (const Operation& operation: operations) {
        const bool works  = operate(operation);
        aoc::part1       += static_cast<size_t>(works) * operation.result();
        aoc::print("total: {}, operation: {}", works, std::string(operation));
    }
}

void run_part_2() {}

} // namespace

void aoc::run() {
    std::ifstream          instream{aoc::file::day_stream()};
    std::vector<Operation> operations{};
    for (std::string line; std::getline(instream, line);) {
        operations.emplace_back(line);
    }
    run_part_1(operations);
    run_part_2();
}
