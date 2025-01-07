#include <cmath>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "util.h"

using namespace std;

constexpr int64_t digits(int64_t value) { return log10(value) + 1; }
constexpr int64_t concat(int64_t sum, int64_t value) { return sum * powl(10, digits(value)) + value; }

template <int part>
bool solveable(auto iter, auto end, int64_t sum, int64_t target) {
    if (iter == end or sum > target) return sum == target;
    bool result = solveable<part>(iter + 1, end, sum + (*iter), target) or
                  solveable<part>(iter + 1, end, sum * (*iter), target);
    if constexpr (part == 2) { result |= solveable<part>(iter + 1, end, concat(sum, (*iter)), target); }
    return result;
}

template <int part>
bool solveable(vector<int64_t> values, int64_t target) {
    return solveable<part>(values.cbegin() + 1, values.cend(), *values.cbegin(), target);
}

template <int part>
int64_t solve(pair<vector<int64_t>, int64_t> p) {
    return solveable<part>(p.first, p.second) ? p.second : 0;
}

int main(int argc, char *argv[]) {
    auto ps = split(read(cin), "\n") | views::transform([](const auto &s) {
                  auto input = split(s, ":");
                  return make_pair(split<int64_t>(input.back()), stoll(input.front()));
              });
    println("Part1: {}", transform_reduce(ps.begin(), ps.end(), 0l, plus(), solve<1>));
    println("Part2: {}", transform_reduce(ps.begin(), ps.end(), 0l, plus(), solve<2>));
    return 0;
}
