#include "util.h"

namespace {

const std::string_view fmt_str{"| {:<12} | {:>3} | {:>3} | {:>8} | {:>10} |"};
const std::string_view num_and_sep{"01234567890,"};

bool is_num_or_sep(char cha) {
    return (cha == ',' || (cha >= '0' && cha <= '9'));
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
        const bool        a_len{sta_sep <= 3 && sta_sep >= 1};
        const bool        b_len{sep_end <= 3 && sep_end >= 1};
        const bool        is_valid = (mul_s) && bet_s && end_s && a_len && b_len;
        if (is_valid) {
            const size_t aval{std::stoul({str_ptr, sta + 1, sta_sep})};
            const size_t bval{std::stoul({str_ptr, sep + 1, sep_end})};
            aoc::part1 += aval * bval;
        }
        idx = line.find("mul(", idx + 1);
    }
}

bool do_enabled{true}; //NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

size_t get_next(const std::string& line, size_t idx) {
    const size_t next_mul  = line.find("mul(", idx + 1);
    const size_t next_do   = line.find("do()", idx + 1);
    const size_t next_dont = line.find("don't()", idx + 1);
    const size_t retval    = aoc::math::min(next_mul, next_do, next_dont); //NOLINT(*-vararg)
    if (retval == next_do) {
        do_enabled = true;
    }
    if (retval == next_dont) {
        do_enabled = false;
    }
    return retval;
}

void run_part2(const std::string& line) {
    size_t idx{get_next(line, 0)};
    while (true) {
        const size_t sta     = {idx + 3};
        const size_t sep     = {line.find(',', idx + 1)};
        const size_t end     = {line.find_first_not_of(num_and_sep, sta + 1)};
        const size_t str_len = {line.size()};
        if (idx >= str_len || sta >= str_len || sep >= str_len || end >= str_len) {
            return;
        }
        const char* const str_ptr  = {line.data()};
        const size_t      sta_sep  = {sep - sta - 1};
        const size_t      sep_end  = {end - sep - 1};
        const std::string betwn    = {str_ptr, sta + 1, end - sta - 1};
        const bool        bet_s    = {std::all_of(betwn.begin(), betwn.end(), is_num_or_sep)};
        const bool        mul_s    = {std::string(str_ptr, idx, 4) == "mul("};
        const bool        end_s    = {line[end] == ')'};
        const bool        a_len    = {sta_sep <= 3 && sta_sep >= 1};
        const bool        b_len    = {sep_end <= 3 && sep_end >= 1};
        const bool        is_valid = (mul_s) && bet_s && end_s && a_len && b_len;
        if (is_valid && do_enabled) {
            const size_t      aval      = {std::stoul({str_ptr, sta + 1, sta_sep})};
            const size_t      bval      = {std::stoul({str_ptr, sep + 1, sep_end})};
            const size_t      mul       = {aval * bval};
            const std::string full_line = {str_ptr, idx, 5 + betwn.size()};
            aoc::print(fmt_str, full_line, aval, bval, mul, aoc::part2);
            aoc::part2 += mul;
        }
        idx = get_next(line, idx);
        if (idx >= line.size()) {
            return;
        }
    }
}

} // namespace

void aoc::run() {
    aoc::print(fmt_str, "Parsed", "a", "b", "Result", "Total");
    const std::string full_file = aoc::string::slurp(aoc::file::day_stream(3));
    run_part1(full_file);
    run_part2(full_file);
}
