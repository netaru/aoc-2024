#include <algorithm>
#include <bitset>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <print>
#include <ranges>
#include <vector>

#include "util.h"

using namespace std;

poses expand(poses input, poses &corners) {
    poses output;
    for (auto i : input) {
        pos p = i * 2l;
        output.insert(p);
        for (pos dir : ordinal) { corners.emplace(p + dir); }
    }
    return output;
}

struct region {
    int   perimiter = 0;
    poses area;

    int sides() {
        poses corners, upscale = expand(area, corners);
        int   sides = 0;
        for (auto c : corners) {
            bitset<4> bits, first(0b1010), second(0b0101);
            for (const auto [u, p] : ordinal | vs::enumerate) { bits[u] = upscale.contains(c + p); }
            if (bits.count() == 1 or bits.count() == 3) {
                sides += 1;
            } else if (bits.count() == 2 and (bits == first or bits == second)) {
                sides += 2;
            }
        }
        return sides;
    }
};

struct garden {
    plane          pl;
    vector<region> rs;
    poses          regions_search;

    garden(istream &is) : pl(is) { solve(); }

    region search(pos in) {
        region result;
        char   c = pl.get(in).value();
        for (deque<pos> q{ in }; q.size(); q.pop_front()) {
            pos p = q.front();
            if (pl.get(p) != c) {
                result.perimiter++;
                continue;
            }
            if (!result.area.insert(p).second) continue;
            for (pos dir : cardinal) { q.push_back(p + dir); }
        }
        rs::copy(result.area, inserter(regions_search, regions_search.begin()));
        return result;
    }

    void solve() {
        for (auto c : pl.chars()) {
            for (pos p : pl.find(c)) {
                if (!regions_search.contains(p)) { rs.emplace_back(search(p)); }
            }
        }
    }

    int price(auto fn) { return transform_reduce(rs.begin(), rs.end(), 0, plus(), fn); }
};

int main(int argc, char *argv[]) {
    garden g(cin);
    println("Part1: {}", g.price([](region r) { return r.area.size() * r.perimiter; }));
    println("Part2: {}", g.price([](region r) { return r.area.size() * r.sides(); }));
    return 0;
}
