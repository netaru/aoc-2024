#include <cstdint>
#include <iostream>
#include <numeric>
#include <print>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "split.h"

using namespace std;

using cache_t = unordered_map<string_view, uint64_t>;
struct linen {
    vector<string> towels, patterns;
    cache_t        cache;

    linen(istream &is) {
        string s;
        getline(is, s);
        s += ",";
        vector<string> list = split(s);
        for (auto t : list) { towels.push_back(t.substr(0, t.size() - 1)); }
        while (getline(is, s)) {
            if (s.size()) { patterns.push_back(s); }
        }
    }

    uint64_t can_make(string_view pattern) {
        if (!pattern.size()) return 1;
        if (cache.contains(pattern)) { return cache[pattern]; }
        uint64_t solutions = 0;
        for (const string towel : towels) {
            if (pattern.substr(0, towel.size()) == towel) { solutions += can_make(pattern.substr(towel.size())); }
        }
        cache[pattern] = solutions;
        return cache[pattern];
    }

    int part1() {
        return accumulate(patterns.begin(), patterns.end(), 0, [&](int acc, string current) {
            return acc + (can_make(current) ? 1 : 0);
        });
    }

    uint64_t part2() {
        return accumulate(patterns.begin(), patterns.end(), 0ull, [&](uint64_t acc, string current) {
            return acc + can_make(current);
        });
    }
};

int main(int argc, char *argv[]) {
    linen l(cin);
    print("Part1: {}\n", l.part1());
    print("Part2: {}\n", l.part2());
    return 0;
}
