#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "split.h"

using pairs = std::vector<std::pair<int, int>>;
using freq  = std::unordered_map<int, int>;

pairs get_pairs(std::vector<int> left, std::vector<int> right) {
    pairs pairs;
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    for (int i = 0; i < left.size(); ++i) { pairs.push_back({ left[i], right[i] }); }
    return pairs;
}

int part1(std::vector<int> left, std::vector<int> right) {
    pairs p = get_pairs(left, right);
    return std::accumulate(p.begin(), p.end(), 0, [](int acc, std::pair<int, int> pair) {
        return std::abs(pair.first - pair.second) + acc;
    });
}

freq get_freq(std::vector<int> v) {
    freq result;
    std::for_each(v.begin(), v.end(), [&result](int value) { result[value]++; });
    return result;
}

int part2(std::vector<int> left, std::vector<int> right) {
    freq f = get_freq(right);
    return std::accumulate(left.begin(), left.end(), 0, [&f](int acc, int value) { return (value * f[value]) + acc; });
}

int main(int argc, char *argv[]) {
    std::string      s;
    std::vector<int> left, right;
    while (std::getline(std::cin, s)) {
        std::vector<int> values = split<int>(s);
        left.push_back(values.front());
        right.push_back(values.back());
    }
    std::cout << "Part1: " << part1(left, right) << "\n";
    std::cout << "Part2: " << part2(left, right) << "\n";
    return 0;
}
