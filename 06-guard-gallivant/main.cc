#include <algorithm>
#include <execution>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "2dgrid.h"

using position = std::complex<int64_t>;

struct stuck {
    int                                        direction = 0;
    position                                   where, p;
    std::vector<position>                      paths{ { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
    std::vector<std::pair<position, position>> visited;
    std::vector<std::string>                   grid;

    stuck(std::vector<std::string> g, position start, position extra) : where(start), p(extra), grid(g) {
        visited.push_back({ where, paths[direction] });
        set(p, '#');
    }

    bool valid(position p) {
        return p.real() >= 0 and p.real() < grid.back().size() and p.imag() >= 0 and p.imag() < grid.size();
    }

    char get(position p) { return valid(p) ? grid[p.real()][p.imag()] : '-'; }

    void set(position p, char c) { grid[p.real()][p.imag()] = c; }

    void update_direction() { direction = (direction + 1) % paths.size(); }

    char peek() { return get(where + paths[direction]); }

    bool contains(std::pair<position, position> pp) {
        return std::find_if(visited.begin(), visited.end(), [&](std::pair<position, position> inner) {
                   return pp.first == inner.first and pp.second == inner.second;
               }) != visited.end();
    }

    bool walk() {
        where += paths[direction];
        std::pair<position, position> pos{ where, paths[direction] };
        if (valid(where)) {
            if (contains(pos)) { return false; }
            visited.push_back(pos);
        }
        return true;
    }

    bool is_stuck() {
        if (p == where) return false;
        while (valid(where)) {
            while (peek() == '#') { update_direction(); }
            if (!walk()) break;
        }
        return valid(where);
    }
};

struct lab {
    int                          direction = 0;
    position                     where, start;
    std::unordered_set<position> visited;
    std::vector<position>        paths{ { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
    std::vector<std::string>     grid;

    lab(std::istream &is) {
        std::string s;
        for (int i = 0; std::getline(is, s); ++i) {
            for (int j = 0; j < s.size(); ++j) {
                if (s[j] == '^') {
                    where = { i, j };
                    start = where;
                    s[j]  = '.';
                    visited.insert(where);
                }
            }
            grid.push_back(s);
        }
    }

    bool valid(position p) {
        return p.real() >= 0 and p.real() < grid.back().size() and p.imag() >= 0 and p.imag() < grid.size();
    }

    char get(position p) { return valid(p) ? grid[p.real()][p.imag()] : '-'; }

    void update_direction() { direction = (direction + 1) % paths.size(); }

    char peek() { return get(where + paths[direction]); }

    void walk() {
        where += paths[direction];
        if (valid(where)) { visited.insert(where); }
    }

    int part1() {
        while (valid(where)) {
            while (peek() == '#') { update_direction(); }
            walk();
        }
        return visited.size();
    }

    int part2() const {
        std::vector<stuck> extra = std::accumulate(
                visited.begin(), visited.end(), std::vector<stuck>{}, [this](std::vector<stuck> acc, position p) {
                    acc.emplace_back(grid, start, p);
                    return acc;
                });
        return std::count_if(std::execution::par, extra.begin(), extra.end(), [](stuck s) { return s.is_stuck(); });
    }
};

int main(int argc, char *argv[]) {
    lab l(std::cin);
    std::cout << "Part1: " << l.part1() << "\n";
    std::cout << "Part2: " << l.part2() << "\n";
    return 0;
}
