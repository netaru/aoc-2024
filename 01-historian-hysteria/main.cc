#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <ranges>
#include <vector>

#include "util.h"

using namespace std;

int part1(span<int> left, span<int> right) {
    return rs::fold_left(vs::zip_transform([](int l, int r) { return abs(l - r); }, left, right), 0, plus());
}

int part2(span<int> left, span<int> right) {
    auto frequencies = rs::fold_left(right, map<int, int>{}, [](auto m, int value) {
        m[value]++;
        return m;
    });
    return rs::fold_left(vs::transform(left, [&](int v) { return v * frequencies[v]; }), 0, plus());
}

int main(int argc, char *argv[]) {
    vector<int> left, right, values = read<int>(cin);
    for (auto &value : values | vs::stride(2)) left.push_back(value);
    for (auto &value : values | vs::drop(1) | vs::stride(2)) right.push_back(value);
    rs::sort(left);
    rs::sort(right);
    println("Part 1: {}", part1(left, right));
    println("Part 2: {}", part2(left, right));
    return 0;
}
