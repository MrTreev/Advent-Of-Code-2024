#include "util.h"
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

namespace {
std::pair<std::vector<int64_t>, std::vector<int64_t>>
get_vecs(std::ifstream& in_stream) {
    std::vector<int64_t> left;
    std::vector<int64_t> right;
    int64_t              a_in{};
    int64_t              b_in{};
    while (in_stream >> a_in >> b_in) {
        left.push_back(a_in);
        right.push_back(b_in);
    }
    std::vector<int64_t> left_sorted{left};
    std::vector<int64_t> right_sorted{right};
    std::ranges::sort(left_sorted);
    std::ranges::sort(right_sorted);
    return std::make_pair(left, right);
}
} // namespace

void aoc::run() {
    std::ifstream in_stream{aoc::file::day_stream()};
    const auto [left, right] = get_vecs(in_stream);
    for (size_t i = 0; i < left.size(); ++i) {
        aoc::part1 += static_cast<size_t>(std::abs(left[i] - right[i]));
    }

    for (const int64_t lval: left) {
        aoc::part2 += static_cast<size_t>(
            lval * std::count(right.begin(), right.end(), lval)
        );
    }
}
