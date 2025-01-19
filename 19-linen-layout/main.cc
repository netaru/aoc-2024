#include <algorithm>
#include <iostream>
#include <numeric>
#include <print>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "util.h"

using namespace std;

using cache_t = unordered_map<string_view, u64>;
struct linen {
    vector<string> input, towels, patterns;
    cache_t        cache;

    linen(istream &is)
        : input(split(read(is), "\n\n")), towels(split(input[0], ",")), patterns(split(input[1], "\n")) {}

    u64 can_make(string_view pattern) {
        if (pattern.empty()) { return 1; }
        if (cache.contains(pattern)) { return cache[pattern]; }
        u64 solutions = 0;
        for (const string towel : towels | vs::filter([&](string s) { return pattern.substr(0, s.size()) == s; }))
            solutions += can_make(pattern.substr(towel.size()));
        return cache[pattern] = solutions;
    }

    u64 part1() {
        return rs::fold_left(patterns | vs::transform([&](auto p) { return can_make(p) ? 1 : 0; }), 0ul, plus());
    }

    u64 part2() {
        return rs::fold_left(patterns | vs::transform([&](auto p) { return can_make(p); }), 0ul, plus());
    }
};

int main(int argc, char *argv[]) {
    linen l(cin);
    println("Part1: {}", l.part1());
    println("Part2: {}", l.part2());
    return 0;
}
