#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "split.h"

using report  = std::vector<int>;
using reports = std::vector<report>;

bool is_sorted(report r) {
    return std::is_sorted(r.begin(), r.end()) ||
           std::is_sorted(r.begin(), r.end(), [](int lhs, int rhs) { return lhs > rhs; });
}

bool element_difference_check(report r) {
    for (int i = 0; i < r.size() - 1; i++) {
        int value = std::abs(r[i] - r[i + 1]);
        if (value > 3 || value == 0) { return false; }
    }
    return true;
}

bool part1_predicate(report r) { return is_sorted(r) && element_difference_check(r); }

bool part2_predicate(report r) {
    bool safe = part1_predicate(r);
    if (!safe) {
        for (int i = 0; i < r.size(); i++) {
            report tmp = r;
            tmp.erase(tmp.begin() + i);
            if (safe = part1_predicate(tmp); safe) break;
        }
    }
    return safe;
}

reports parse() {
    reports     rs;
    std::string s;
    while (std::getline(std::cin, s)) rs.push_back(split<int>(s));
    return rs;
}

int main(int argc, char *argv[]) {
    reports rs = parse();
    std::cout << "Part1: " << std::count_if(rs.begin(), rs.end(), part1_predicate) << "\n";
    std::cout << "Part2: " << std::count_if(rs.begin(), rs.end(), part2_predicate) << "\n";
    return 0;
}
