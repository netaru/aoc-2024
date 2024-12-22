#include <cmath>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <print>
#include <vector>

using namespace std;

struct prng {
    int64_t seed, current;

    prng(int64_t _seed) : seed(_seed), current(_seed) {}

    constexpr int64_t mix(int64_t n, int64_t v) { return n ^ v; }
    constexpr int64_t mul(int64_t n) { return n << 6; }
    constexpr int64_t mul2(int64_t n) { return n << 11; }
    constexpr int64_t div(int64_t n) { return n >> 5; }
    constexpr int64_t prune(int64_t n) { return n & 16777215; }

    int64_t tick() {
        current = prune(mix(current, mul(current)));
        current = prune(mix(current, div(current)));
        current = prune(mix(current, mul2(current)));
        return current;
    }

    int64_t next(int64_t n = 1) {
        for (int64_t i = 0; i < n; ++i) { tick(); }
        return current;
    }
};

int main(int argc, char *argv[]) {
    int64_t         sum = 0;
    vector<int64_t> nums{ istream_iterator<int64_t>(cin), {} };
    for (auto v : nums) {
        prng    rng(v);
        int64_t value = rng.next(2000);
        print("{}: {}\n", v, value);
        sum += value;
    }
    print("Part1: {}\n", sum);
    return 0;
}
