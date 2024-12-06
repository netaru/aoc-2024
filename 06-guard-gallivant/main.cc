#include <algorithm>
#include <execution>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_set>
#include <vector>

#include "2dgrid.h"

using position = std::complex<int64_t>;
using history  = std::unordered_set<position>;

struct visit {
    position where, direction;

    visit(position w, position d) : where(w), direction(d) {}
    friend bool operator==(const visit& lhs, const visit& rhs) {
        return lhs.where == rhs.where and lhs.direction == rhs.direction;
    }
};

template <>
struct std::hash<visit> {
    std::size_t operator()(const struct visit& v) const {
        return std::hash<int64_t>()(v.where.real()) ^ (std::hash<int64_t>()(v.where.imag()) << 1) ^
               std::hash<int64_t>()(v.direction.real() << 2) ^ (std::hash<int64_t>()(v.direction.imag()) << 3);
    }
};

struct lab {
    int                       direction = 0;
    position                  where;
    std::vector<position>     paths{ { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
    std::unordered_set<visit> visited;
    std::vector<std::string>  grid;

    lab(std::istream& is) {
        std::string s;
        for (int i = 0; std::getline(is, s); ++i) {
            for (int j = 0; j < s.size(); ++j) {
                if (s[j] == '^') {
                    where = { i, j };
                    visited.insert({ where, paths[direction] });
                }
            }
            grid.push_back(s);
        }
    }

    lab(position w, std::vector<std::string> g) : where(w), grid(g) { visited.insert({ where, paths[direction] }); }

    bool valid(position p) {
        return p.real() >= 0 and p.real() < grid.back().size() and p.imag() >= 0 and p.imag() < grid.size();
    }

    char get(position p) { return valid(p) ? grid[p.real()][p.imag()] : '-'; }

    void set(position p, char c) { grid[p.real()][p.imag()] = c; }

    void update_direction() { direction = (direction + 1) % paths.size(); }

    char peek() { return get(where + paths[direction]); }

    bool walk() {
        where += paths[direction];
        visit v{ where, paths[direction] };
        if (valid(where)) {
            if (visited.contains(v)) { return false; }
            visited.insert(v);
        }
        return true;
    }

    void traverse() {
        while (valid(where)) {
            while (peek() == '#') { update_direction(); }
            if (!walk()) break;
        }
    }
};

struct stuck : public lab {
    position block;

    stuck(std::vector<std::string> g, position start, position b) : lab(start, g), block(b) { set(block, '#'); }

    bool is_stuck() {
        if (block == where) return false;
        traverse();
        return valid(where);
    }
};

struct guard : lab {
    position start;
    history  uniq;

    guard(std::istream& is) : lab(is), start(where) {}

    int part1() {
        traverse();
        uniq = std::accumulate(visited.begin(), visited.end(), history{}, [](history acc, visit v) {
            acc.insert(v.where);
            return acc;
        });
        return uniq.size();
    }

    int part2() const {
        std::vector<stuck> ss = std::accumulate(
                uniq.begin(), uniq.end(), std::vector<stuck>{}, [this](std::vector<stuck> acc, position block) {
                    acc.emplace_back(grid, start, block);
                    return acc;
                });
        return std::count_if(std::execution::par, ss.begin(), ss.end(), [](stuck s) { return s.is_stuck(); });
    }
};

int main(int argc, char* argv[]) {
    guard l(std::cin);
    std::cout << "Part1: " << l.part1() << "\n";
    std::cout << "Part2: " << l.part2() << "\n";
    return 0;
}
