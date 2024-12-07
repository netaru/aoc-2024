#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <istream>
#include <numeric>
#include <string>
#include <vector>

#include "split.h"

using input_t  = std::vector<std::string>;
using int_t    = std::uint64_t;
using values_t = std::vector<int_t>;

struct problem {
    input_t  input;
    int_t    target;
    values_t values;

    constexpr size_t digits(const int_t value) const { return std::log10(value) + 1; }
    constexpr int_t  concat(const int_t sum, const int_t value) const {
        return sum * std::powl(10, digits(value)) + value;
    }

    template <int part>
    bool solveable(values_t::const_iterator iter, const int_t sum) const {
        if (iter == values.cend() or sum > target) return sum == target;
        if constexpr (part == 1) {
            return solveable<part>(iter + 1, sum + (*iter)) or solveable<part>(iter + 1, sum * (*iter));
        } else {
            return solveable<part>(iter + 1, sum + (*iter)) or solveable<part>(iter + 1, sum * (*iter)) or
                   solveable<part>(iter + 1, concat(sum, (*iter)));
        }
    }

    template <int part = 1>
    bool solveable() const {
        return solveable<part>(values.cbegin() + 1, *values.cbegin());
    }

    problem(std::string s)
        : input(split(s, ':')), target(std::stoull(input.front())), values(split<int_t>(input.back())) {}
};

struct calc {
    std::vector<problem> problems;

    calc(std::istream &is) {
        std::string s;
        while (std::getline(is, s)) { problems.push_back({ s }); }
    }

    int_t part1() {
        return std::accumulate(problems.cbegin(), problems.cend(), int_t(0), [](int_t acc, problem p) {
            if (p.solveable()) { acc += p.target; }
            return acc;
        });
    }

    int_t part2() {
        return std::accumulate(problems.cbegin(), problems.cend(), 0ull, [](int_t acc, problem p) {
            return acc + (p.solveable<2>() ? p.target : 0);
        });
    }
};

int main(int argc, char *argv[]) {
    calc calculator(std::cin);
    std::cout << "Part1: " << calculator.part1() << "\n";
    std::cout << "Part2: " << calculator.part2() << "\n";
    return 0;
}
