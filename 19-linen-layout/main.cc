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
    vector<string> towels, patterns;
    cache_t        cache;

    linen(istream &is) {
        auto input = split(read(is), "\n\n");
        towels     = split(input[0], ",");
        patterns   = split(input[1], "\n");
    }

    u64 can_make(string_view pattern) {
        if (!pattern.size()) return 1;
        if (cache.contains(pattern)) { return cache[pattern]; }
        u64 solutions = 0;
        for (const string towel : towels | views::filter([&](string s) { return pattern.substr(0, s.size()) == s; })) {
            solutions += can_make(pattern.substr(towel.size()));
        }
        return cache[pattern] = solutions;
    }

    u64 part1() {
        return transform_reduce(
                patterns.begin(), patterns.end(), 0ul, plus(), [&](string c) { return can_make(c) ? 1 : 0; });
    }

    u64 part2() {
        return transform_reduce(patterns.begin(), patterns.end(), 0ul, plus(), [&](string c) { return can_make(c); });
    }
};

int main(int argc, char *argv[]) {
    linen l(cin);
    println("Part1: {}", l.part1());
    println("Part2: {}", l.part2());
    return 0;
}
