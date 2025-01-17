#include <algorithm>
#include <cmath>
#include <execution>
#include <iostream>
#include <iterator>
#include <numeric>
#include <print>
#include <ranges>
#include <unordered_map>
#include <vector>

#include "util.h"

using namespace std;

using bananas_t = unordered_map<i64, i64>;

struct monkey {
    i64 current, price, total, window;

    bananas_t bananas;

    monkey(i64 seed) : current(seed), price(current % 10), total(0), window(seed + 9) {
        for (i64 prev = price; total < 3; ++total, prev = price) {
            price  = tick() % 10;
            window = ((window << 5) | ((price - prev) + 9)) & 0xfffff;
        }
    }

    constexpr i64 mix(i64 n, i64 v) { return n ^ v; }
    constexpr i64 mul(i64 n, i64 v = 6) { return n << v; }
    constexpr i64 div(i64 n) { return n >> 5; }
    constexpr i64 prune(i64 n) { return n & 16777215; }

    i64 tick() {
        current        = prune(mix(current, mul(current)));
        current        = mix(current, div(current));
        return current = prune(mix(current, mul(current, 11)));
    }

    i64 next(i64 n = 1) {
        for (i64 prev = price; total < n; ++total, prev = price) {
            price  = tick() % 10;
            window = ((window << 5) | ((price - prev) + 9)) & 0xfffff;
            if (!bananas.contains(window)) { bananas[window] += price; }
        }
        return current;
    }
};

int main(int argc, char *argv[]) {
    vector<monkey> monkeys{ istream_iterator<i64>(cin), {} };
    i64            part1 = transform_reduce(
            execution::par, monkeys.begin(), monkeys.end(), 0l, plus(), [](monkey &m) { return m.next(2000); });

    bananas_t bananas;
    for (const auto &m : monkeys) {
        for (const auto &[key, value] : m.bananas) bananas[key] += value;
    }

    print("Part1: {}\n", part1);
    print("Part2: {}\n", rs::max(vs::values(bananas)));
    return 0;
}
