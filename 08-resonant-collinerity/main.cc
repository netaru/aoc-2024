#include <iostream>

#include "util.h"

using namespace std;

struct station {
    plane<char> grid;
    unordered_map<char, poses> f;

    station(istream &is) : grid(is) {
        auto ch = grid.values();
        ch.erase(ch.find('.'));
        for (const auto &c : ch) { f[c] = grid.find(c); }
    }

    template <int part>
    poses antinodes(pos origin, poses others) {
        auto filt = [&](pos other) { return other != origin; };
        auto trans = [&](pos other) -> poses {
            if constexpr (part == 1) {
                if (pos delta = other - origin, node = origin + delta + delta; grid.valid(node)) return { node };
            } else {
                return grid.line(origin, other);
            }
            return {};
        };
        return rs::fold_left(others | vs::filter(filt) | vs::transform(trans), poses{}, dave::merge);
    }

    template <int part>
    poses collect(pair<char, poses> p) {
        auto &[c, input] = p;
        return rs::fold_left(
                input | vs::transform([&](auto where) { return antinodes<part>(where, input); }), poses{}, dave::merge);
    }

    template <int part = 1>
    size_t get_antinodes() {
        return rs::fold_left(f | vs::transform([&](auto p) { return collect<part>(p); }), poses{}, dave::merge).size();
    }
};

int main(int argc, char *argv[]) {
    station s(cin);
    println("Part1: {}", s.get_antinodes<1>());
    println("Part2: {}", s.get_antinodes<2>());
    return 0;
}
