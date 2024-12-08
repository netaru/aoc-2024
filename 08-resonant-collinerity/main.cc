#include <algorithm>
#include <cmath>
#include <complex>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <istream>
#include <iterator>
#include <numeric>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "2dgrid.h"

using positions = std::unordered_set<position>;
using freqs     = std::unordered_map<char, positions>;

freqs parse(std::istream &is) {
    std::string s;
    freqs       f;
    int         y = 0;
    while (std::getline(is, s)) {
        for (int x = 0; x < s.size(); ++x) {
            if (s[x] != '.') { f[s[x]].insert(position{ x, y }); }
        }
        ++y;
    }
    return f;
}

const size_t max = 50;

bool inbounds(position p) { return p.real() >= 0 and p.real() < max and p.imag() >= 0 and p.imag() < max; }
void add(position delta, position at, positions &which) {
    for (; inbounds(at); at += delta) { which.insert(at); }
}

template <int part>
positions antinodes(position origin, positions others) {
    positions result;
    for (auto other : others) {
        if (other == origin) continue;
        if constexpr (part == 1) {
            position delta = other - origin;
            position node  = origin + delta + delta;
            if (inbounds(node)) { result.insert(node); }
        } else {
            add(other - origin, origin, result);
            add(origin - other, origin, result);
        }
    }
    return result;
}

template <int part>
positions collect(std::pair<char, positions> p) {
    auto &[ch, input] = p;
    return std::accumulate(input.begin(), input.end(), positions{}, [&](positions acc, position where) {
        positions anti = antinodes<part>(where, input);
        std::copy(anti.begin(), anti.end(), std::inserter(acc, acc.begin()));
        return acc;
    });
}

template <int part = 1>
size_t get_antinodes(const freqs &frequencies) {
    return std::accumulate(
                   frequencies.begin(),
                   frequencies.end(),
                   positions{},
                   [](positions acc, std::pair<char, positions> p) {
                       positions anti = collect<part>(p);
                       std::copy(anti.begin(), anti.end(), std::inserter(acc, acc.begin()));
                       return acc;
                   })
            .size();
}

int main(int argc, char *argv[]) {
    freqs f = parse(std::cin);
    std::cout << "Part1: " << get_antinodes(f) << "\n";
    std::cout << "Part2: " << get_antinodes<2>(f) << "\n";
    return 0;
}
