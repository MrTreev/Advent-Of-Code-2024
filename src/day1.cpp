#include "util.h"
#include <algorithm>
#include <iostream>
#include <vector>

void aoc::run() {
    std::ifstream        in_stream{aoc::file::day_stream(1)};
    std::vector<int64_t> left;
    std::vector<int64_t> right;

    int64_t a_in{};
    int64_t b_in{};
    while (in_stream >> a_in >> b_in) {
        left.push_back(a_in);
        right.push_back(b_in);
    }

    const size_t         vecsize{left.size()};
    std::vector<int64_t> left_sorted{left};
    std::vector<int64_t> right_sorted{right};
    std::sort(left_sorted.begin(), left_sorted.end());
    std::sort(right_sorted.begin(), right_sorted.end());

    for (size_t i = 0; i < vecsize; ++i) {
        aoc::part1 += static_cast<size_t>(std::abs(left[i] - right[i]));
    }

    for (const int64_t lval: left) {
        aoc::part2 += static_cast<size_t>(
            lval * std::count(right.begin(), right.end(), lval)
        );
    }
}
