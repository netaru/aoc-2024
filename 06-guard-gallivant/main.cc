#include <algorithm>
#include <execution>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_set>
#include <vector>

#include "2dgrid.h"

struct lab {
    int                      direction = 0;
    position                 where, start, block;
    std::vector<position>    paths{ { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
    std::vector<std::string> grid;
    visit_history            visited;
    history                  uniq;

    lab(std::istream& is) {
        std::string s;
        for (int i = 0; std::getline(is, s); ++i) {
            for (int j = 0; j < s.size(); ++j) {
                if (s[j] == '^') {
                    where = start = { i, j };
                    uniq.insert(where);
                }
            }
            grid.push_back(s);
        }
    }

    lab(std::vector<std::string> g, position w, position b) : where(w), grid(g), block(b) { set(block, '#'); }

    bool valid(position p) {
        return p.real() >= 0 and p.real() < grid.back().size() and p.imag() >= 0 and p.imag() < grid.size();
    }

    char get(position p) { return valid(p) ? grid[p.real()][p.imag()] : '-'; }
    void set(position p, char c) { grid[p.real()][p.imag()] = c; }
    char peek() { return get(where + paths[direction]); }

    bool update_direction() {
        bool should_continue = true;
        if (visit v{ where, paths[direction] }; !visited.contains(v)) {
            visited.insert(v);
        } else {
            should_continue = false;
        }
        direction = (direction + 1) % paths.size();
        return should_continue;
    }

    template <int part>
    void walk() {
        where += paths[direction];
        if constexpr (part == 1) {
            if (valid(where)) { uniq.insert(where); }
        }
    }

    template <int part = 1>
    void traverse() {
        while (valid(where)) {
            while (peek() == '#')
                if (!update_direction()) return;
            walk<part>();
        }
    }

    bool is_stuck() {
        if (block == where) return false;
        traverse<2>();
        return valid(where);
    }

    int part1() {
        traverse();
        return uniq.size();
    }

    int part2() const {
        std::vector<lab> labs =
                std::accumulate(uniq.begin(), uniq.end(), std::vector<lab>{}, [this](std::vector<lab> acc, position b) {
                    acc.emplace_back(grid, start, b);
                    return acc;
                });
        return std::count_if(std::execution::par, labs.begin(), labs.end(), [](lab l) { return l.is_stuck(); });
    }
};

int main(int argc, char* argv[]) {
    lab l(std::cin);
    std::cout << "Part1: " << l.part1() << "\n";
    std::cout << "Part2: " << l.part2() << "\n";
    return 0;
}
