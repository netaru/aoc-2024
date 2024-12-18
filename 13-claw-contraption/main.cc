#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <numeric>
#include <print>
#include <string>
#include <vector>
#include "util.h"

using namespace std;

int64_t score(double ax, double ay, double bx, double by, double xtarget, double ytarget) {
    double ca = (xtarget * by - ytarget * bx) / (ax * by - ay * bx);
    double cb = (xtarget - ax * ca) / bx;
    if (modf(ca, &ax) == 0 and modf(cb, &ax) == 0) { return ca * 3 + cb; }
    return 0;
}

int main(int argc, char *argv[]) {
    string                    s{ std::istream_iterator<char>(cin), {} };
    vector<int64_t>           input = ints<int64_t>(s);
    vector<array<int64_t, 6>> nums;

    for (size_t u = 0, i = 0; u < input.size(); u += 6, i++) {
        nums.push_back({});
        for (size_t k = 0; k < 6; ++k) { nums[i][k] = input[u + k]; }
    }

    int64_t score1 = accumulate(nums.begin(), nums.end(), 0, [](int acc, auto n) {
        return acc + score(n[0], n[1], n[2], n[3], n[4], n[5]);
    });
    int64_t score2 = accumulate(nums.begin(), nums.end(), 0ull, [](int64_t acc, auto n) {
        return acc + score(n[0], n[1], n[2], n[3], n[4] + 10000000000000, n[5] + 10000000000000);
    });
    print("Part1: {}\n", score1);
    print("Part2: {}\n", score2);
    return 0;
}
