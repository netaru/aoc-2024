#include <algorithm>
#include <iostream>
#include <print>
#include <utility>

#include "util.h"

using namespace std;

struct lab {
    plane    p;
    pos      start, where, delta;
    dhistory visited;
    history  hist;

    lab(istream& is) : p(is), start(*p.find('^').begin()), where(start), delta(0, -1) { hist.insert(where); }

    template <int part = 1>
    auto walk() {
        while (p.valid(where)) {
            if constexpr (part == 1)
                if (p.valid(where)) { hist.insert(where); }
            if (p.get(where + delta) == '#') {
                delta = clockwise(delta);
                if constexpr (part == 2)
                    if (!visited.insert({ where, delta }).second) break;
            } else {
                where += delta;
            }
        }
        if constexpr (part == 1) { return hist.size(); }
    }

    bool is_stuck(pos block) {
        where = start;
        delta = pos{ 0, -1 };
        visited.clear();
        p.set(block, '#');
        walk<2>();
        p.set(block, '.');
        return p.valid(where);
    }

    int cycles() {
        hist.erase(hist.find(start));
        auto fn = [&](pos block) { return is_stuck(block); };
        return count_if(hist.begin(), hist.end(), fn);
    }
};

int main(int argc, char* argv[]) {
    lab l(cin);
    println("Part1: {}", l.walk());
    println("Part2: {}", l.cycles());
    return 0;
}
