#include <cstdint>
#include <deque>
#include <execution>
#include <iostream>
#include <istream>
#include <numeric>
#include <string>
#include <vector>

#include "split.h"

using int_t    = std::uint64_t;
using values_t = std::deque<int_t>;

int_t pop_front(values_t &values) {
    int_t r = values.front();
    values.pop_front();
    return r;
}

struct problem {
    int_t    target;
    values_t values;

    int_t concat(int_t sum, int_t value) {
        for (size_t u = std::to_string(value).size(); u; --u) sum *= 10;
        return sum + value++;
    }

    template <int part>
    bool solveable(values_t remaining, int_t sum) {
        if (!remaining.size()) return sum == target;
        int_t value = pop_front(remaining);
        if constexpr (part == 2) {
            return solveable<part>(remaining, sum + value) || solveable<part>(remaining, sum * value) ||
                   solveable<part>(remaining, concat(sum, value));
        } else {
            return solveable<part>(remaining, sum + value) || solveable<part>(remaining, sum * value);
        }
    }

    template <int part = 1>
    bool solveable() {
        values_t values_copy = values;
        int_t    sum         = pop_front(values_copy);
        return solveable<part>(values_copy, sum);
    }

    problem(std::string s) {
        std::vector<std::string> ss = split(s, ':');
        target                      = std::stoll(ss.front());
        std::vector<std::string> is = split(ss.back());
        values = std::accumulate(is.cbegin(), is.cend(), values_t{}, [](values_t acc, std::string value) {
            acc.push_back(std::stoull(value));
            return acc;
        });
    }
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
        return std::accumulate(problems.begin(), problems.end(), int_t(0), [](int_t acc, problem p) {
            if (p.solveable<2>()) { acc += p.target; }
            return acc;
        });
    }
};

int main(int argc, char *argv[]) {
    calc calculator(std::cin);
    std::cout << "Part1: " << calculator.part1() << "\n";
    std::cout << "Part2: " << calculator.part2() << "\n";
    return 0;
}
