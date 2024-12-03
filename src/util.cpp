#include "util.h"
#include <iostream>
#include <vector>

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

void aoc::string::clean_string(std::string& str) {
    std::erase_if(str, [](const char& item) {
        return (item == '\n');
    });
}
