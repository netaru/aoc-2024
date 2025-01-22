#include <iostream>

#include "util.h"

using namespace std;

int main(int argc, char *argv[]) {
    plane p(cin);
    auto isAMS = [&](pos where, pos delta) { return dave::sort(p.slice(where, delta, 3)) == "AMS"; };
    auto fn1 = [&](pos where) {
        return rs::count_if(compass, [&](pos delta) { return p.slice(where, delta, 4) == "XMAS"; });
    };
    auto fn2 = [&](pos where) { return isAMS(where + dave::NE, dave::SW) and isAMS(where + dave::NW, dave::SE); };
    println("Part1: {}", rs::fold_left(p.find('X') | vs::transform(fn1), 0, plus()));
    println("Part2: {}", rs::count_if(p.find('A'), fn2));
    return 0;
}
