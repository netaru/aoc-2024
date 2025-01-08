#include <deque>
#include <functional>
#include <iostream>
#include <numeric>
#include <print>
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
        for (auto dir : cardinal)
            if (pos next = at + dir; p.get(next) == seek) { q.push_back({ next, seek }); }
    }
    return sum;
}

template <int part>
int score(const plane &p, const poses &zeros) {
    auto fn = bind(bfs<part>, p, placeholders::_1);
    return transform_reduce(zeros.begin(), zeros.end(), 0, plus(), fn);
}

int main(int argc, char *argv[]) {
    plane p(cin);
    auto  zeroes = p.find('0');
    println("Part1: {}", score<1>(p, zeroes));
    println("Part2: {}", score<2>(p, zeroes));
    return 0;
}
