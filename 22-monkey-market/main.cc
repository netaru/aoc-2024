#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <numeric>
#include <print>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

using pair_t   = pair<int64_t, int64_t>;
using banana_t = unordered_map<int64_t, int64_t>;

struct monkey {
    int64_t         current;
    vector<int64_t> prices, changes;

    monkey(int64_t seed) : current(seed) { prices.push_back(current % 10); }

    constexpr int64_t mix(int64_t n, int64_t v) { return n ^ v; }
    constexpr int64_t mul(int64_t n, int64_t v = 6) { return n << v; }
    constexpr int64_t div(int64_t n) { return n >> 5; }
    constexpr int64_t prune(int64_t n) { return n & 16777215; }

    int64_t tick() {
        current        = prune(mix(current, mul(current)));
        current        = prune(mix(current, div(current)));
        return current = prune(mix(current, mul(current, 11)));
    }

    int64_t next(int64_t n = 1) {
        for (int64_t i = 0; i < n; ++i) {
            prices.push_back(tick() % 10);
            changes.push_back(prices.back() - *(prices.end() - 2));
        }
        return current;
    }

    template <int offset = 0>
    int64_t seq(int64_t index) const {
        return (changes[index - offset] + 9) << (offset * 5);
    }
    void sell(banana_t &bananas) const {
        unordered_set<int64_t> seen;
        for (int i = 3; i < changes.size(); ++i) {
            int64_t history = seq(i) | seq<1>(i) | seq<2>(i) | seq<3>(i);
            if (!seen.contains(history)) {
                seen.insert(history);
                bananas[history] += prices[i + 1];
            }
        }
    }
};

int main(int argc, char *argv[]) {
    banana_t bananas;
    int64_t  part1 = accumulate(istream_iterator<int64_t>(cin), {}, 0l, [&](int64_t acc, int64_t value) {
        monkey m(value);
        acc += m.next(2000);
        m.sell(bananas);
        return acc;
    });
    print("Part1: {}\n", part1);
    print("Part2: {}\n", ranges::max(views::values(bananas)));
    return 0;
}
