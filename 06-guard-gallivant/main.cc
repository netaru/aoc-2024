#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "2dgrid.h"

struct lab {
    int                      direction = 0;
    position                 where, start;
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

    bool valid(position p) {
        return p.real() >= 0 and p.real() < grid.back().size() and p.imag() >= 0 and p.imag() < grid.size();
    }

    char get(position p) { return valid(p) ? grid[p.real()][p.imag()] : '-'; }
    void set(position p, char c) { grid[p.real()][p.imag()] = c; }
    char peek() { return get(where + paths[direction]); }

    template <int part>
    bool update_direction() {
        if constexpr (part == 1) {
            direction = (direction + 1) % paths.size();
            return true;
        }
        if (visit v{ where, paths[direction] }; !visited.contains(v)) {
            visited.insert(v);
            direction = (direction + 1) % paths.size();
            return true;
        } else {
            return false;
        }
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
                if (!update_direction<part>()) return;
            walk<part>();
        }
    }

    bool is_stuck(position block) {
        where = start;
        visited.clear();
        direction = 0;
        set(block, '#');
        traverse<2>();
        set(block, '.');
        return valid(where);
    }

    int part1() {
        traverse();
        return uniq.size();
    }

    int part2() {
        uniq.erase(uniq.find(start));
        return std::count_if(uniq.begin(), uniq.end(), [&](position block) { return is_stuck(block); });
    }
};

int main(int argc, char* argv[]) {
    lab l(std::cin);
    std::cout << "Part1: " << l.part1() << "\n";
    std::cout << "Part2: " << l.part2() << "\n";
    return 0;
}
