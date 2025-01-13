#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <ranges>
#include <vector>

#include "util.h"

using namespace std;

int part1(span<int> left, span<int> right) {
    return ranges::fold_left(views::zip_transform([](int l, int r) { return abs(l - r); }, left, right), 0, plus());
}

int part2(span<int> left, span<int> right) {
    auto frequencies = ranges::fold_left(right, map<int, int>{}, [](auto m, int value) {
        m[value]++;
        return m;
    });
    return ranges::fold_left(views::transform(left, [&](int v) { return v * frequencies[v]; }), 0, plus());
}

int main(int argc, char *argv[]) {
    vector<int> left, right, values = read<int>(cin);
    for (auto &value : values | views::stride(2)) left.push_back(value);
    for (auto &value : values | views::drop(1) | views::stride(2)) right.push_back(value);
    ranges::sort(left);
    ranges::sort(right);
    println("Part 1: {}", part1(left, right));
    println("Part 2: {}", part2(left, right));
    return 0;
}
