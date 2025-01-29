#include "util.h"
#include <algorithm>
#include <ranges>
#include <string>
#include <vector>

namespace {
bool badtst(const int aval, const int bval) {
    const int diff = std::abs(aval - bval);
    return (diff > 3 || diff < 1);
}

bool is_sorted_one_way(const std::vector<int>& vec) {
    return (
        std::ranges::is_sorted(vec)
        || std::ranges::is_sorted(std::ranges::reverse_view(vec))
    );
}

bool all_good(const std::vector<int>& vec) {
    if (!is_sorted_one_way(vec)) {
        return false;
    }
    for (size_t i = 1; i < vec.size(); ++i) {
        if (badtst(vec[i - 1], vec[i])) {
            return false;
        }
    }
    return true;
}

std::vector<int> vec_except(const std::vector<int>& vec, size_t idx) {
    std::vector<int> outvec{};
    outvec.reserve(vec.size() - 1);
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i != idx) {
            outvec.push_back(vec[i]);
        }
    }
    return outvec;
}

bool subone_good(const std::vector<int>& vec) {
    if (all_good(vec)) {
        return true;
    }
    for (size_t i = 1; i < vec.size(); ++i) {
        if ((badtst(vec[i - 1], vec[i]))) {
            if (all_good(vec_except(vec, i))) {
                return true;
            }
        }
    }
    return false;
}

} // namespace

void aoc::run() {
    std::ifstream    in_stream{aoc::file::day_stream()};
    std::string      line;
    std::string      item;
    std::vector<int> items;
    while (std::getline(in_stream, line)) {
        std::stringstream linestream{line};
        items.clear();
        while (linestream >> item) {
            items.push_back(std::stoi(item));
        }
        if (all_good(items)) {
            aoc::part1 += 1;
        }
        if (subone_good(items)) {
            aoc::part2 += 1;
        }
    }
}
