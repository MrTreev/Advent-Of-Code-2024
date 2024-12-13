#include "util.h"

namespace {
constexpr size_t AXIS_SIZE{12}; // 12 for test, 50 for full

class Roof: public aoc::util::Grid<AXIS_SIZE, AXIS_SIZE> {
    using Grid = aoc::util::Grid<AXIS_SIZE, AXIS_SIZE>;

public:
    explicit Roof(std::ifstream& infile)
        : Grid(infile) {}
};

void run_part_1() {}

void run_part_2() {}
} // namespace

void aoc::run() {
    std::ifstream instream{aoc::file::day_stream(8)}; // NOLINT(*-magic-numbers)
    Roof          roof{instream};
    aoc::print(std::string(roof));
    run_part_1();
    run_part_2();
}
