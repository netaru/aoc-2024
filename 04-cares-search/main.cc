#include <algorithm>
#include <complex>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

#include "util.h"

using namespace std;

bool isAMS(const plane &p, pos where, pos delta) { return dave::sort(p.slice(where, delta, 3)) == "AMS"; }

int main(int argc, char *argv[]) {
    plane p(cin);
    auto  fn1 = [&](pos where) {
        return count_if(compass.begin(), compass.end(), [&](pos delta) { return p.slice(where, delta, 4) == "XMAS"; });
    };
    auto fn2 = [&](pos where) { return isAMS(p, where + dave::NE, dave::SW) and isAMS(p, where + dave::NW, dave::SE); };
    auto xs = p.find('X'), as = p.find('A');
    println("Part1: {}", transform_reduce(xs.begin(), xs.end(), 0, plus(), fn1));
    println("Part2: {}", count_if(as.begin(), as.end(), fn2));
    return 0;
}
