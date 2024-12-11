#include <cmath>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <print>
#include <string>
#include <utility>
#include <vector>

using namespace std;

using int_t       = int64_t;
using cache_key_t = pair<int_t, int_t>;
using cache_t     = map<cache_key_t, int_t>;

cache_t cache;
int_t   solve(int_t value, int_t blinks) {
    cache_key_t key{ value, blinks };
    if (blinks == 0) return 1;
    if (cache.contains(key)) return cache[key];

    int_t result(0);
    if (value == 0) {
        result = solve(1, blinks - 1);
    } else if (string s = to_string(value); s.size() % 2 == 0) {
        result = solve(stoul(s.substr(0, s.size() / 2)), blinks - 1) + solve(stoul(s.substr(s.size() / 2)), blinks - 1);
    } else {
        result = solve(value * 2024, blinks - 1);
    }
    cache[key] = result;
    return result;
}

auto fn(const int_t steps) {
    return [steps](int_t acc, int_t value) { return acc + solve(value, steps); };
}

int main(int argc, char* argv[]) {
    vector<int_t> values{ istream_iterator<int_t>{ cin }, {} };
    print("Part 1: {}\n", accumulate(values.begin(), values.end(), 0ull, fn(25)));
    print("Part 2: {}\n", accumulate(values.begin(), values.end(), 0ull, fn(75)));
    return 0;
}
