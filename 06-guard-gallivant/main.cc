#include <algorithm>
#include <functional>
#include <iostream>
#include <ostream>
#include <print>
#include <string>
#include <utility>
#include <vector>

#include "2dgrid.h"

using namespace std;

struct lab {
    position       where, start, delta;
    vector<string> grid;
    visit_history  visited;
    history        uniq;

    lab(istream& is) : delta(-1, 0) {
        string s;
        for (int i = 0; getline(is, s); ++i) {
            for (int j = 0; j < s.size(); ++j) {
                if (s[j] == '^') {
                    where = start = { i, j };
                    uniq.insert(where);
                }
            }
            grid.push_back(s);
        }
    }

    bool valid(position p) const {
        return p.real() >= 0 and p.real() < grid.back().size() and p.imag() >= 0 and p.imag() < grid.size();
    }

    char get(position p) const { return valid(p) ? grid[p.real()][p.imag()] : '-'; }
    void set(position p, char c) { grid[p.real()][p.imag()] = c; }
    char peek() const { return get(where + delta); }

    template <int part>
    bool update_direction() {
        delta = position{ delta.imag(), -delta.real() };
        if constexpr (part == 1) { return true; }
        return visited.insert({ where, delta }).second;
    }

    template <int part = 1>
    auto walk() {
        bool run = true;
        while (valid(where) and run) {
            if constexpr (part == 1)
                if (valid(where)) { uniq.insert(where); }
            if (peek() == '#')
                run = update_direction<part>();
            else
                where += delta;
        }
        if constexpr (part == 1) { return uniq.size(); }
    }

    bool is_stuck(position block) {
        where = start;
        delta = position{ -1, 0 };
        visited.clear();
        set(block, '#');
        walk<2>();
        set(block, '.');
        return valid(where);
    }

    int cycles() {
        uniq.erase(uniq.find(start));
        auto fn = bind(mem_fn(&lab::is_stuck), this, placeholders::_1);
        return count_if(uniq.begin(), uniq.end(), fn);
    }
};

int main(int argc, char* argv[]) {
    lab l(cin);
    cout << "Part1: " << l.walk() << "\n";
    cout << "Part2: " << l.cycles() << "\n";
    return 0;
}
