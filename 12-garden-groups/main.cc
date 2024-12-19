#include <algorithm>
#include <bitset>
#include <cstddef>
#include <deque>
#include <iostream>
#include <iterator>
#include <numeric>
#include <unordered_set>
#include <vector>

#include "2dgrid.h"

using namespace std;
using queue_t = deque<position>;

position pop(queue_t &q) {
    position p = q.front();
    q.pop_front();
    return p;
}

using uset = unordered_set<position>;

uset expand(uset input) {
    uset output;
    for (position p : input) { output.emplace(p.real() * 2, p.imag() * 2); }
    return output;
}

auto corner_directions = vector<position>{ { -1, -1 }, { 1, -1 }, { -1, 1 }, { 1, 1 } };
struct region {
    char ch;
    int  perimiter = 0;
    uset area;

    int sides() {
        uset upscale = expand(area), corners;
        for (auto u : upscale) {
            for (position dir : corner_directions) { corners.emplace(u.real() + dir.real(), u.imag() + dir.imag()); }
        }
        int sides = 0;
        for (auto c : corners) {
            bitset<4> bits, first(0b0110), second(0b1001);
            for (size_t u = 0; u < corner_directions.size(); ++u) {
                position dir = c + corner_directions[u];
                bits[u]      = upscale.contains(dir);
            }
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
    vector<string> grid;
    vector<region> regions;
    set            regions_search;

    garden(istream &is) {
        string s;
        while (getline(is, s)) { grid.push_back(s); }
        solve();
    }

    bool valid(position p) {
        return p.real() >= 0 and p.real() < grid.back().size() and p.imag() >= 0 and p.imag() < grid.size();
    }

    char get(position p) { return valid(p) ? grid[p.imag()][p.real()] : '-'; }

    region search(position in) {
        region  result;
        queue_t q{ in };
        char    c = get(in);
        result.ch = c;
        while (q.size()) {
            position p     = pop(q);
            char     inner = get(p);
            if (inner != c) {
                result.perimiter++;
                continue;
            }
            if (result.area.contains(p)) continue;
            result.area.insert(p);
            for (position dir : vector<position>{ { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } }) { q.push_back(p + dir); }
        }
        copy(result.area.begin(), result.area.end(), inserter(regions_search, regions_search.begin()));
        return result;
    }

    void solve() {
        for (int x = 0; x < grid.size(); ++x) {
            for (int y = 0; y < grid.size(); ++y) {
                position p{ y, x };
                if (regions_search.contains(p)) continue;
                regions.emplace_back(search(p));
            }
        }
    }

    int part1() {
        return accumulate(regions.begin(), regions.end(), 0, [](int acc, region r) {
            return acc + (r.area.size() * r.perimiter);
        });
    }

    int part2() {
        return accumulate(
                regions.begin(), regions.end(), 0, [](int acc, region r) { return acc + (r.area.size() * r.sides()); });
    }
};

int main(int argc, char *argv[]) {
    garden g(cin);
    cout << "Part1: " << g.part1() << "\n";
    cout << "Part2: " << g.part2() << "\n";
    return 0;
}
