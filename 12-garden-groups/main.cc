#include <algorithm>
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

struct region {
    char                    ch;
    int                     perimiter = 0;
    unordered_set<position> area;
};

struct garden {
    vector<string> grid;
    vector<region> regions;
    set            regions_search;

    garden(istream &is) {
        string s;
        while (getline(is, s)) { grid.push_back(s); }
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

    int score() {
        solve();
        return accumulate(regions.begin(), regions.end(), 0, [](int acc, region r) {
            return acc + (r.area.size() * r.perimiter);
        });
    }
};

int main(int argc, char *argv[]) {
    garden g(cin);
    cout << "Part1: " << g.score() << "\n";
    return 0;
}
