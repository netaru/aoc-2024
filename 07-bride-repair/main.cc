#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <istream>
#include <numeric>
#include <ostream>
#include <ranges>
#include <string>
#include <vector>

#include "util.h"

using namespace std;

struct problem {
    vector<string>  input;
    int64_t         target;
    vector<int64_t> values;

    constexpr size_t  digits(int64_t value) const { return log10(value) + 1; }
    constexpr int64_t concat(int64_t sum, int64_t value) const { return sum * powl(10, digits(value)) + value; }

    template <int part>
    bool solveable(auto iter, int64_t sum) const {
        if (iter == values.cend() or sum > target) return sum == target;
        bool result = solveable<part>(iter + 1, sum + (*iter)) or solveable<part>(iter + 1, sum * (*iter));
        if constexpr (part == 2) { result |= solveable<part>(iter + 1, concat(sum, (*iter))); }
        return result;
    }

    template <int part = 1>
    bool solveable() const {
        return solveable<part>(values.cbegin() + 1, *values.cbegin());
    }

    problem(string s) : input(split(s, ":")), target(stoll(input.front())), values(split<int64_t>(input.back())) {}
};

int main(int argc, char *argv[]) {
    auto ps  = split(read(cin), "\n") | views::transform([](const auto &s) { return problem{ s }; });
    auto fn1 = [](problem p) { return p.solveable<1>() ? p.target : 0; };
    auto fn2 = [](problem p) { return p.solveable<2>() ? p.target : 0; };
    println("Part1: {}", transform_reduce(ps.begin(), ps.end(), 0l, plus(), fn1));
    println("Part2: {}", transform_reduce(ps.begin(), ps.end(), 0l, plus(), fn2));
    return 0;
}
