#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <vector>

using namespace std;

int part1(const vector<int> &left, const vector<int> &right) {
    return ranges::fold_left(
            views::zip_transform([](int l, int r) { return abs(l - r); }, left, right), 0, plus<int>());
}

int part2(const vector<int> &left, const vector<int> &right) {
    auto f = accumulate(right.begin(), right.end(), map<int, int>{}, [](auto acc, int value) {
        acc[value]++;
        return acc;
    });
    return ranges::fold_left(views::transform(left, [&](int v) { return v * f[v]; }), 0, plus<int>());
}

void parse(vector<int> &left, vector<int> &right) {
    int first, second;
    while (cin >> first >> second) {
        left.push_back(first);
        right.push_back(second);
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());
}

int main(int argc, char *argv[]) {
    vector<int> left, right;
    parse(left, right);
    cout << "Part1: " << part1(left, right) << "\n";
    cout << "Part2: " << part2(left, right) << "\n";
    return 0;
}
