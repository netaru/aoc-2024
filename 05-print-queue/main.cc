#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <ranges>
#include <set>
#include <vector>
#include "util.h"

using namespace std;
using pages_t = vector<vector<int>>;
constexpr int middle(const auto &p) { return p[(p.size() / 2)]; }

int main(int argc, char *argv[]) {
    map<int, set<int>> rules;
    auto comp{ [&rules](int lhs, int rhs) { return rules[lhs].contains(rhs); } };
    auto in = split(read(cin), "\n\n");
    for (const auto &rule : split(in[0], "\n") | vs::transform([](auto s) { return split<int>(s, "|"); }))
        rules[rule.front()].insert(rule.back());
    pages_t pages = split(in[1], "\n") | vs::transform([](auto s) { return split<int>(s, ","); }) | rs::to<pages_t>();
    auto fn1 = [&](auto p) { return rs::is_sorted(p, comp) ? middle(p) : 0; };
    auto fn2 = [&](auto p) { return rs::is_sorted(p, comp) ? 0 : middle(dave::sort(p, comp)); };
    println("Part1: {}", rs::fold_left(pages | vs::transform(fn1), 0, plus()));
    println("Part2: {}", rs::fold_left(pages | vs::transform(fn2), 0, plus()));
    return 0;
}
