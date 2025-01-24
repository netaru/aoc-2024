#include <cstdlib>
#include <deque>
#include <iostream>
#include <print>
#include <tuple>
#include <unordered_map>

#include "util.h"

using namespace std;

struct cpu {
    plane<char> pl;
    pos start, end;

    unordered_map<pos, int> nodes;

    cpu(istream &is) : pl(is), start(pl.find_first('S')), end(pl.find_first('E')) {
        nodes.emplace(start, 0);
        bfs();
    }

    void bfs() {
        for (deque<tuple<pos, size_t>> q{ { start, 0 } }; q.size(); q.pop_front()) {
            const auto [current, ms] = q.front();
            for (pos npos : add(current, cardinal)) {
                if (nodes.contains(npos) or pl.get(npos) == '#') continue;
                nodes.emplace(npos, ms + 1);
                q.emplace_back(npos, ms + 1);
                break;
            }
        }
    }

    pair<int, int> cheat() {
        int part1 = 0, part2 = 0;
        for (auto iter1 = nodes.cbegin(), iter2 = iter1; iter1 != nodes.cend(); iter1++, iter2 = iter1) {
            for (iter2++; iter2 != nodes.cend(); iter2++) {
                const auto [pos1, cost1] = *iter1;
                const auto [pos2, cost2] = *iter2;

                int distance = manhattan(pos1, pos2);
                if (distance <= 20 and abs(cost1 - cost2) >= 100 + distance) {
                    part2++;
                    if (distance <= 2) part1++;
                }
            }
        }
        return { part1, part2 };
    }
};

int main(int argc, char *argv[]) {
    cpu c(cin);
    const auto [part1, part2] = c.cheat();
    println("Part1: {}", part1);
    println("Part2: {}", part2);
    return 0;
}
