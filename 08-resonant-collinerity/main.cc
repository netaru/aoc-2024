#include <algorithm>
#include <cmath>
#include <complex>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <istream>
#include <iterator>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "util.h"

using poss  = std::unordered_set<pos>;
using freqs = std::unordered_map<char, poss>;

freqs parse(std::istream &is) {
    plane p(is);
    auto  ch = p.chars();
    ch.erase(ch.find('.'));
    freqs f;
    for (const auto &c : ch) { f[c] = p.locate(c); }
    return f;
}

const size_t max = 50;

bool inbounds(pos p) { return p.real() >= 0 and p.real() < max and p.imag() >= 0 and p.imag() < max; }
void add(pos delta, pos at, poss &which) {
    for (; inbounds(at); at += delta) { which.insert(at); }
}

template <int part>
poss antinodes(pos origin, poss others) {
    poss result;
    for (auto other : others) {
        if (other == origin) continue;
        if constexpr (part == 1) {
            pos delta = other - origin;
            pos node  = origin + delta + delta;
            if (inbounds(node)) { result.insert(node); }
        } else {
            add(other - origin, origin, result);
            add(origin - other, origin, result);
        }
    }
    return result;
}

template <int part>
poss collect(std::pair<char, poss> p) {
    auto &[ch, input] = p;
    return std::accumulate(input.begin(), input.end(), poss{}, [&](poss acc, pos where) {
        poss anti = antinodes<part>(where, input);
        std::copy(anti.begin(), anti.end(), std::inserter(acc, acc.begin()));
        return acc;
    });
}

template <int part = 1>
size_t get_antinodes(const freqs &frequencies) {
    return std::accumulate(
                   frequencies.begin(),
                   frequencies.end(),
                   poss{},
                   [](poss acc, std::pair<char, poss> p) {
                       poss anti = collect<part>(p);
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
