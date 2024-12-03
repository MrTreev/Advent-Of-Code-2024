#include "util.h"
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ifstream        in_stream{aoc::file::day_stream(1)};
    std::vector<int64_t> left;
    std::vector<int64_t> right;

    int64_t a;
    int64_t b;
    while (in_stream >> a >> b) {
        left.push_back(a);
        right.push_back(b);
    }

    const size_t         vecsize{left.size()};
    std::vector<int64_t> left_sorted{left};
    std::vector<int64_t> right_sorted{right};
    std::sort(left_sorted.begin(), left_sorted.end());
    std::sort(right_sorted.begin(), right_sorted.end());

    int64_t part1{0};
    for (size_t i = 0; i < vecsize; ++i) {
        part1 += std::abs(left[i] - right[i]);
    }

    int64_t part2{0};
    for (const int64_t lval: left) {
        part2 += (lval * std::count(right.begin(), right.end(), lval));
    }

    aoc::output::print_part(1, part1);
    aoc::output::print_part(2, part2);
}
