#include <cmath>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <print>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "util.h"

using namespace std;

using cache_key_t = pair<i64, i64>;
using cache_t = map<cache_key_t, i64>;

cache_t cache;
i64 solve(i64 value, i64 blinks) {
    cache_key_t key{ value, blinks };
    if (blinks == 0) return 1;
    if (cache.contains(key)) return cache[key];

    i64 result(0);
    if (value == 0) {
        result = solve(1, blinks - 1);
    } else if (string s = to_string(value); s.size() % 2 == 0) {
        result = solve(stoul(s.substr(0, s.size() / 2)), blinks - 1) + solve(stoul(s.substr(s.size() / 2)), blinks - 1);
    } else {
        result = solve(value * 2024, blinks - 1);
    }
    return cache[key] = result;
}

template <int steps>
auto fn() {
    return [](i64 value) { return solve(value, steps); };
}

int main(int argc, char* argv[]) {
    vector<i64> xs{ istream_iterator<i64>{ cin }, {} };
    println("Part1: {}", rs::fold_left(xs | vs::transform(fn<25>()), 0ll, plus()));
    println("Part2: {}", rs::fold_left(xs | vs::transform(fn<75>()), 0ll, plus()));
    return 0;
}
