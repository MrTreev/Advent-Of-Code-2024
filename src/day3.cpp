#include "util.h"

namespace {

const std::string_view fmt_str{"| {:<12} | {:>3} | {:>3} | {:>8} | {:>10} |"};
const std::string_view num_and_sep{"01234567890,"};

bool is_num_or_sep(char c) {
    return (c == ',' || (c >= '0' && c <= '9'));
}

void run_part1(const std::string& line) {
    size_t idx{line.find("mul(", 0)};
    while (true) {
        const size_t      sta{idx + 3};
        const size_t      sep{line.find(',', idx + 1)};
        const size_t      end{line.find_first_not_of(num_and_sep, sta + 1)};
        const size_t      str_len{line.size()};
        const char* const str_ptr{line.data()};
        if (idx > str_len || sta > str_len || sep > str_len || end > str_len) {
            return;
        }

        const size_t      sta_sep{sep - sta - 1};
        const size_t      sep_end{end - sep - 1};
        const std::string betwn{str_ptr, sta + 1, end - sta - 1};
        const bool        bet_s{std::all_of(betwn.begin(), betwn.end(), is_num_or_sep)};
        const bool        mul_s{std::string(str_ptr, idx, 4) == "mul("};
        const bool        end_s{line[end] == ')'};
        const bool        a_len{!(sta_sep > 3 || sta_sep < 1)};
        const bool        b_len{!(sep_end > 3 || sep_end < 1)};
        const bool        is_valid = (mul_s) && bet_s && end_s && a_len && b_len;
        if (is_valid) {
            const size_t a{std::stoul({str_ptr, sta + 1, sta_sep})};
            const size_t b{std::stoul({str_ptr, sep + 1, sep_end})};
            aoc::part1 += a * b;
        }
        idx = line.find("mul(", idx + 1);
    }
}

size_t get_next(const std::string& line, size_t idx) {
    aoc::debug("");
    const size_t next_mul{line.find("mul(", idx + 1)};
    const size_t next_do{line.find("do()", idx + 1)};
    const size_t next_dont{line.find("don't()", idx + 1)};
    if (!(next_mul < line.size())) {
        return next_mul;
    }
    if ((next_dont < next_mul) && (next_do > next_mul)) {
        aoc::debug("skipping next_mul  {}", next_mul);
        aoc::debug("skipping next_do   {}", next_do);
        aoc::debug("skipping next_dont {}", next_dont);
        get_next(line, next_do);
    } else {
        aoc::debug("next_mul  {}", next_mul);
        aoc::debug("next_do   {}", next_do);
        aoc::debug("next_dont {}", next_dont);
    }
    return next_mul;
}

void run_part2(const std::string& line) {
    size_t idx{get_next(line, 0)};
    while (true) {
        const size_t      sta{idx + 3};
        const size_t      sep{line.find(',', idx + 1)};
        const size_t      end{line.find_first_not_of(num_and_sep, sta + 1)};
        const size_t      str_len{line.size()};
        const char* const str_ptr{line.data()};
        if (!(idx < str_len && sta < str_len && sep < str_len && end < str_len)) {
            return;
        }

        const size_t      sta_sep{sep - sta - 1};
        const size_t      sep_end{end - sep - 1};
        const std::string betwn{str_ptr, sta + 1, end - sta - 1};
        const bool        bet_s{std::all_of(betwn.begin(), betwn.end(), is_num_or_sep)};
        const bool        mul_s{std::string(str_ptr, idx, 4) == "mul("};
        const bool        end_s{line[end] == ')'};
        const bool        a_len{!(sta_sep > 3 || sta_sep < 1)};
        const bool        b_len{!(sep_end > 3 || sep_end < 1)};
        const bool        is_valid = (mul_s) && bet_s && end_s && a_len && b_len;
        if (is_valid) {
            const size_t      a{std::stoul({str_ptr, sta + 1, sta_sep})};
            const size_t      b{std::stoul({str_ptr, sep + 1, sep_end})};
            const size_t      mul{a * b};
            const std::string full_line = {str_ptr, idx, 5 + betwn.size()};
            aoc::debug(full_line);
            // aoc::debug(fmt_str, full_line, a, b, mul, aoc::part2);
            aoc::part2 += mul;
        }
        idx = get_next(line, idx);
    }
}

} // namespace

void aoc::run() {
    auto        in_stream{aoc::file::day_stream(3)};
    std::string line;
    aoc::debug(fmt_str, "Parsed", "a", "b", "Result", "Total");
    const std::string full_file = aoc::string::slurp(in_stream);
    run_part1(full_file);
    run_part2(full_file);
}
