#include <deque>
#include <iostream>
#include <istream>
#include <numeric>
#include <print>
#include <utility>

#include "util.h"

using namespace std;

using queue = deque<pair<pos, char>>;

struct top {
    plane p;
    poses starts;

    top(istream &is) : p(is), starts(p.find('0')) {}

    template <int part>
    int bfs(pos where) {
        int     sum = 0;
        history visited;

        queue q{ { where, '0' } };
        while (q.size()) {
            const auto &[at, value] = pop(q);
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
    int score() {
        return transform_reduce(starts.begin(), starts.end(), 0, plus(), [&](pos p) { return bfs<part>(p); });
    }
};

int main(int argc, char *argv[]) {
    top t(cin);
    println("Part1: {}", t.score<1>());
    println("Part2: {}", t.score<2>());
    return 0;
}
