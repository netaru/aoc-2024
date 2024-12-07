#include <cstddef>
#include <cstdint>
#include <execution>
#include <iostream>
#include <istream>
#include <numeric>
#include <string>
#include <vector>

#include "split.h"

using int_t    = std::uint64_t;
using values_t = std::vector<int_t>;

struct problem {
    int_t    target;
    values_t values;

    size_t digit(int_t value) {
        size_t d = 0;
        while (value) {
            ++d;
            value /= 10;
        }
        return d;
    }

    int_t concat(int_t sum, int_t value) {
        for (size_t u = digit(value); u; --u) sum *= 10;
        return sum + value;
    }

    template <int part>
    bool solveable(values_t::const_iterator iter, int_t sum) {
        if (iter == values.cend()) return sum == target;
        if constexpr (part == 2) {
            return solveable<part>(iter + 1, sum + (*iter)) || solveable<part>(iter + 1, sum * (*iter)) ||
                   solveable<part>(iter + 1, concat(sum, (*iter)));
        } else {
            return solveable<part>(iter + 1, sum + (*iter)) || solveable<part>(iter + 1, sum * (*iter));
        }
    }

    template <int part = 1>
    bool solveable() {
        return solveable<part>(values.cbegin() + 1, *values.cbegin());
    }

    problem(std::string s) {
        std::vector<std::string> ss = split(s, ':');

        target = std::stoll(ss.front());
        values = split<int_t>(ss.back());
    }
};

int_t reduce(int_t lhs, int_t rhs) { return lhs + rhs; }
int_t reduce(problem lhs, int_t rhs) { return (lhs.solveable<2>() ? lhs.target : 0) + rhs; }
int_t reduce(int_t lhs, problem rhs) { return (rhs.solveable<2>() ? rhs.target : 0) + lhs; }
int_t reduce(problem lhs, problem rhs) {
    return (lhs.solveable<2>() ? lhs.target : 0) + (rhs.solveable<2>() ? rhs.target : 0);
}

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
        return std::reduce(std::execution::par, problems.cbegin(), problems.cend(), int_t(0), [](auto lhs, auto rhs) {
            return reduce(lhs, rhs);
        });
    }
};

int main(int argc, char *argv[]) {
    calc calculator(std::cin);
    std::cout << "Part1: " << calculator.part1() << "\n";
    std::cout << "Part2: " << calculator.part2() << "\n";
    return 0;
}
