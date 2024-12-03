#include "utility/types.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

int main() {
    const auto    filename = std::filesystem::absolute("day1.txt");
    std::ifstream in_stream{filename};

    std::vector<uint32_t> left;
    std::vector<uint32_t> right;

    uint32_t a, b;
    while (in_stream >> a >> b) {
        left.push_back(a);
        right.push_back(b);
    }
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    size_t       sum     = 0;
    const size_t vecsize = left.size();
    for (size_t i = 0; i < vecsize; ++i) {
        sum += (left[i] + right[i]);
    }
    std::cout << "Sum: " << std::to_string(sum) << std::endl;
}
