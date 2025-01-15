#include <deque>
#include <functional>
#include <iostream>
#include <print>
#include <ranges>
#include <utility>

#include "util.h"

using namespace std;

template <int part>
int bfs(const plane &p, pos where) {
    int     sum = 0;
    history visited;
    for (deque<pair<pos, char>> q{ { where, '0' } }; !q.empty(); q.pop_front()) {
        const auto &[at, value] = q.front();
        if constexpr (part == 1) {
            if (const auto [iter, inserted] = visited.insert(at); !inserted) continue;
        }
        if (value == '9') {
            sum++;
            continue;
        }

        char seek = value + 1;
        for (auto next : add(at, cardinal))
            if (p.get(next) == seek) { q.push_back({ next, seek }); }
    }
    return sum;
}

template <int part>
int score(const plane &p, const poses &zs) {
    auto fn = [&](pos ps) { return bfs<part>(p, ps); };
    return rs::fold_left(zs | vs::transform(fn), 0, plus());
}

int main(int argc, char *argv[]) {
    plane p(cin);
    auto  zs = p.find('0');
    println("Part1: {}", score<1>(p, zs));
    println("Part2: {}", score<2>(p, zs));
    return 0;
}
