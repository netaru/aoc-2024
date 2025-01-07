#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <set>
#include <vector>
#include "util.h"

using namespace std;
constexpr int middle(const auto &p) { return p[(p.size() / 2)]; }

int main(int argc, char *argv[]) {
    map<int, set<int>>  rules;
    vector<vector<int>> pages;
    auto                comp{ [&rules](int lhs, int rhs) { return rules[lhs].contains(rhs); } };
    auto                input = split(read(cin), "\n\n");
    for (const auto &rule : split(input[0], "\n") | views::transform([](auto s) { return split<int>(s, "|"); }))
        rules[rule.front()].insert(rule.back());
    for (const auto &s : split(input[1], "\n")) { pages.push_back(split<int>(s, ",")); }
    auto fn1 = [&](auto p) { return ranges::is_sorted(p, comp) ? middle(p) : 0; };
    auto fn2 = [&](auto p) { return ranges::is_sorted(p, comp) ? 0 : middle(dave::sort(p, comp)); };
    println("Part1: {}", transform_reduce(pages.begin(), pages.end(), 0, plus(), fn1));
    println("Part2: {}", transform_reduce(pages.begin(), pages.end(), 0, plus(), fn2));
    return 0;
}
