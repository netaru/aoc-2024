#include <algorithm>
#include <iostream>
#include <optional>
#include <print>
#include <utility>

#include "util.h"

using namespace std;

template <int part>
auto walk(plane& plane, pos start, optional<pos> block = {}) {
    pos      where = start, delta(0, -1);
    history  hist;
    dhistory visited;
    if (block.has_value()) plane.set(block.value(), '#');
    while (plane.valid(where)) {
        if constexpr (part == 1)
            if (plane.valid(where)) { hist.insert(where); }
        if (plane.get(where + delta) == '#') {
            delta = clockwise(delta);
            if constexpr (part == 2)
                if (!visited.insert({ where, delta }).second) break;
        } else {
            where += delta;
        }
    }
    if (block.has_value()) plane.set(block.value(), '.');
    if constexpr (part == 1) { return hist; }
    if constexpr (part == 2) { return where; }
}

auto is_stuck(plane& plane, pos start, auto hist) {
    hist.erase(hist.find(start));
    auto fn = [&](pos block) { return plane.valid(walk<2>(plane, start, block)); };
    return ranges::count_if(hist, fn);
}

int main(int argc, char* argv[]) {
    plane plane(cin);
    pos   start = plane.find_first('^');
    auto  hist  = walk<1>(plane, start);
    println("Part1: {}", hist.size());
    println("Part2: {}", is_stuck(plane, start, hist));
    return 0;
}
