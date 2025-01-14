#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
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
    for (const auto &rule : split(input[0], "\n") | vs::transform([](auto s) { return split<int>(s, "|"); }))
        rules[rule.front()].insert(rule.back());
    for (const auto &s : split(input[1], "\n")) { pages.push_back(split<int>(s, ",")); }
    auto fn1 = [&](auto p) { return rs::is_sorted(p, comp) ? middle(p) : 0; };
    auto fn2 = [&](auto p) { return rs::is_sorted(p, comp) ? 0 : middle(dave::sort(p, comp)); };
    println("Part1: {}", rs::fold_left(pages | vs::transform(fn1), 0, plus()));
    println("Part2: {}", rs::fold_left(pages | vs::transform(fn2), 0, plus()));
    return 0;
}
