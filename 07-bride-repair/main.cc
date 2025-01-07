#include <cmath>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "util.h"

using namespace std;

constexpr i64 digits(i64 value) { return log10(value) + 1; }
constexpr i64 concat(i64 sum, i64 value) { return sum * powl(10, digits(value)) + value; }

template <int part>
bool solveable(auto iter, auto end, i64 sum, i64 target) {
    if (iter == end or sum > target) return sum == target;
    bool result = solveable<part>(iter + 1, end, sum + (*iter), target) or
                  solveable<part>(iter + 1, end, sum * (*iter), target);
    if constexpr (part == 2) { result |= solveable<part>(iter + 1, end, concat(sum, (*iter)), target); }
    return result;
}

template <int part>
bool solveable(vector<i64> values, i64 target) {
    return solveable<part>(values.cbegin() + 1, values.cend(), *values.cbegin(), target);
}

template <int part>
i64 solve(pair<vector<i64>, i64> p) {
    return solveable<part>(p.first, p.second) ? p.second : 0;
}

int main(int argc, char *argv[]) {
    auto ps = split(read(cin), "\n") | views::transform([](const auto &s) {
                  auto input = split(s, ":");
                  return make_pair(split<i64>(input.back()), stoll(input.front()));
              });
    println("Part1: {}", transform_reduce(ps.begin(), ps.end(), 0l, plus(), solve<1>));
    println("Part2: {}", transform_reduce(ps.begin(), ps.end(), 0l, plus(), solve<2>));
    return 0;
}
