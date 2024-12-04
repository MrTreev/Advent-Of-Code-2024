#include "util.h"
#include <algorithm>
#include <iostream>

bool   aoc::test_mode;
size_t aoc::part1{0};
size_t aoc::part2{0};

void aoc::print(const std::string& str) {
    std::cout << str << std::endl;
}

void aoc::debug(const std::string& str) {
    if (aoc::test_mode) {
        std::cout << str << std::endl;
    }
}

std::string aoc::file::day_txt(uint8_t day) {
    const std::string filename = std::format("out/test/day{}.txt", day);
    std::println(std::cout, "Advent of Code Day {}, filename: {}", day, filename);
    return filename;
}

std::filesystem::path aoc::file::day_path(uint8_t day) {
    return std::filesystem::relative(day_txt(day));
}

std::ifstream aoc::file::day_stream(uint8_t day) {
    return {day_path(day)};
}

void aoc::output::print_part(uint8_t num, const size_t& part) {
    std::println(std::cout, "Part {}: {}", num, part);
}

bool aoc::string::is_numeric(const std::string& str) {
    return std::all_of(str.cbegin(), str.cend(), [](const char& c) {
        return (c >= '0' && c <= '9');
    });
}

size_t aoc::math::max(size_t a, size_t b) {
    return std::max(a, b);
}

size_t aoc::math::min(size_t a, size_t b) {
    return std::min(a, b);
}

size_t aoc::math::max(size_t a, size_t b, size_t args...) {
    return max(std::max(a, b), args);
}

size_t aoc::math::min(size_t a, size_t b, size_t args...) {
    return min(std::min(a, b), args);
}

std::string aoc::string::slurp(std::ifstream& in) {
    std::ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    if (argc > 1) {
        aoc::test_mode = true;
    }
    aoc::run();
    aoc::output::print_part(1, aoc::part1);
    aoc::output::print_part(2, aoc::part2);
}
