#include "util.h"
#include "answers.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
std::atomic<bool>   aoc::test_mode;
std::atomic<size_t> aoc::part1{0};
std::atomic<size_t> aoc::part2{0};

namespace {
uint8_t g_day{0};
}

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

void aoc::print(const std::string& str) {
    std::cout << str << "\n";
}

void aoc::debug(const std::string& str) {
    if (aoc::test_mode) {
        std::cout << str << "\n";
    }
}

std::string aoc::file::day_file(uint8_t day) {
    g_day = day;
    const std::string fname{test_mode ? "test" : "day"};
    const std::string filename =
        std::format("out/test/{}{:02}.txt", fname, day);
    std::println(
        std::cout,
        "Advent of Code Day {}, filename: {}",
        day,
        filename
    );
    return filename;
}

std::filesystem::path aoc::file::day_path(uint8_t day) {
    return std::filesystem::relative(day_file(day));
}

std::filesystem::path aoc::file::day_stream(const std::source_location& loc) {
    const std::string fname{loc.file_name()};
    const uint8_t     day = string::char_to_uint(
        fname.at(fname.size() - 6),
        fname.at(fname.size() - 5)
    );
    return {day_path(day)};
}

void aoc::output::print_part(uint8_t num, const size_t& part) {
    std::println(std::cout, "Part {}: {}", num, part);
}

bool aoc::string::is_numeric(const std::string& str) {
    return std::ranges::all_of(str, [](const char& cha) {
        return (cha >= '0' && cha <= '9');
    });
}

size_t aoc::math::max(size_t aval, size_t bval) {
    return std::max(aval, bval);
}

size_t aoc::math::min(size_t aval, size_t bval) {
    return std::min(aval, bval);
}

size_t aoc::math::max(size_t aval, size_t bval, size_t args...) {
    return max(std::max(aval, bval), args);
}

size_t aoc::math::min(size_t aval, size_t bval, size_t args...) {
    return min(std::min(aval, bval), args);
}

std::string aoc::string::slurp(std::ifstream& instr) {
    std::ostringstream sstr{};
    sstr << instr.rdbuf();
    return sstr.str();
}

uint8_t aoc::string::char_to_uint(char cha) {
    return static_cast<uint8_t>(cha) - static_cast<uint8_t>('0');
}

uint8_t aoc::string::char_to_uint(char cha_1, char cha_2) {
    constexpr auto BASE_10 = 10;
    return (BASE_10 * char_to_uint(cha_1)) + char_to_uint(cha_2);
}

namespace {
size_t get_part_1() { // NOLINT(*-cognitive-complexity)
    // NOLINTBEGIN(*-magic-numbers)
    switch (g_day) {
    case 1:  return aoc::test_mode ? day01::test1 : day01::part1;
    case 2:  return aoc::test_mode ? day02::test1 : day02::part1;
    case 3:  return aoc::test_mode ? day03::test1 : day03::part1;
    case 4:  return aoc::test_mode ? day04::test1 : day04::part1;
    case 5:  return aoc::test_mode ? day05::test1 : day05::part1;
    case 6:  return aoc::test_mode ? day06::test1 : day06::part1;
    case 7:  return aoc::test_mode ? day07::test1 : day07::part1;
    case 8:  return aoc::test_mode ? day08::test1 : day08::part1;
    case 9:  return aoc::test_mode ? day09::test1 : day09::part1;
    case 10: return aoc::test_mode ? day10::test1 : day10::part1;
    case 11: return aoc::test_mode ? day11::test1 : day11::part1;
    case 12: return aoc::test_mode ? day12::test1 : day12::part1;
    case 13: return aoc::test_mode ? day13::test1 : day13::part1;
    case 14: return aoc::test_mode ? day14::test1 : day14::part1;
    case 15: return aoc::test_mode ? day15::test1 : day15::part1;
    case 16: return aoc::test_mode ? day16::test1 : day16::part1;
    case 17: return aoc::test_mode ? day17::test1 : day17::part1;
    case 18: return aoc::test_mode ? day18::test1 : day18::part1;
    case 19: return aoc::test_mode ? day19::test1 : day19::part1;
    case 20: return aoc::test_mode ? day20::test1 : day20::part1;
    case 21: return aoc::test_mode ? day21::test1 : day21::part1;
    case 22: return aoc::test_mode ? day22::test1 : day22::part1;
    case 23: return aoc::test_mode ? day23::test1 : day23::part1;
    case 24: return aoc::test_mode ? day24::test1 : day24::part1;
    case 25: return aoc::test_mode ? day25::test1 : day25::part1;
    default: throw std::invalid_argument(std::format("Bad day: {}", g_day));
    }
    // NOLINTEND(*-magic-numbers)
}

size_t get_part_2() { // NOLINT(*-cognitive-complexity)
    // NOLINTBEGIN(*-magic-numbers)
    switch (g_day) {
    case 1:  return aoc::test_mode ? day01::test2 : day01::part2;
    case 2:  return aoc::test_mode ? day02::test2 : day02::part2;
    case 3:  return aoc::test_mode ? day03::test2 : day03::part2;
    case 4:  return aoc::test_mode ? day04::test2 : day04::part2;
    case 5:  return aoc::test_mode ? day05::test2 : day05::part2;
    case 6:  return aoc::test_mode ? day06::test2 : day06::part2;
    case 7:  return aoc::test_mode ? day07::test2 : day07::part2;
    case 8:  return aoc::test_mode ? day08::test2 : day08::part2;
    case 9:  return aoc::test_mode ? day09::test2 : day09::part2;
    case 10: return aoc::test_mode ? day10::test2 : day10::part2;
    case 11: return aoc::test_mode ? day11::test2 : day11::part2;
    case 12: return aoc::test_mode ? day12::test2 : day12::part2;
    case 13: return aoc::test_mode ? day13::test2 : day13::part2;
    case 14: return aoc::test_mode ? day14::test2 : day14::part2;
    case 15: return aoc::test_mode ? day15::test2 : day15::part2;
    case 16: return aoc::test_mode ? day16::test2 : day16::part2;
    case 17: return aoc::test_mode ? day17::test2 : day17::part2;
    case 18: return aoc::test_mode ? day18::test2 : day18::part2;
    case 19: return aoc::test_mode ? day19::test2 : day19::part2;
    case 20: return aoc::test_mode ? day20::test2 : day20::part2;
    case 21: return aoc::test_mode ? day21::test2 : day21::part2;
    case 22: return aoc::test_mode ? day22::test2 : day22::part2;
    case 23: return aoc::test_mode ? day23::test2 : day23::part2;
    case 24: return aoc::test_mode ? day24::test2 : day24::part2;
    case 25: return aoc::test_mode ? day25::test2 : day25::part2;
    default: throw std::invalid_argument(std::format("Bad day: {}", g_day));
    }
    // NOLINTEND(*-magic-numbers)
}
} // namespace

// NOLINTNEXTLINE(bugprone-exception-escape)
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    if (argc > 1) {
        aoc::test_mode = true;
    }
    aoc::run();
    aoc::output::print_part(1, aoc::part1);
    aoc::output::print_part(2, aoc::part2);
    const auto part1_answer = get_part_1();
    const auto part2_answer = get_part_2();
    if (part1_answer != 0) {
        if (aoc::part1 != part1_answer) {
            const size_t got = aoc::part1;
            const size_t ans = part1_answer;
            std::println(
                std::cerr,
                "Error in Part 1: Got {}, Wanted {}",
                got,
                ans
            );
        } else {
            aoc::print("Part 1 Correct: {}", get_part_1());
        }
    } else {
        aoc::print("No stored answer for part 1, give it a go");
    }
    if (part2_answer != 0) {
        if (aoc::part2 != part2_answer) {
            const size_t got = aoc::part2;
            const size_t ans = part2_answer;
            std::println(
                std::cerr,
                "Error in Part 2: Got {}, Wanted {}",
                got,
                ans
            );
        } else {
            aoc::print("Part 2 Correct: {}", get_part_2());
        }
    } else {
        aoc::print("No stored answer for part 2, give it a go");
    }
}
