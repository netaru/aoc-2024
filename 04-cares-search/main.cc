#include <algorithm>
#include <complex>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

#include "util.h"

using namespace std;

struct station {
    plane       p;
    vector<pos> as, xs;

    station(istream &is) : p(is), as(p.locate('A')), xs(p.locate('X')) {}

    int part1() {
        return transform_reduce(xs.begin(), xs.end(), 0, plus(), [this](pos where) {
            return count_if(
                    compass.begin(), compass.end(), [&](pos delta) { return p.slice(where, delta, 4) == "XMAS"; });
        });
    }

    bool isAMS(pos where, pos delta) { return dave::sort(p.slice(where, delta, 3)) == "AMS"; }
    int  part2() {
        return count_if(as.begin(), as.end(), [&](pos p) { return isAMS(p + NE, SW) and isAMS(p + NW, SE); });
    }
};

int main(int argc, char *argv[]) {
    station station(cin);
    cout << "Part1: " << station.part1() << "\n";
    cout << "Part2: " << station.part2() << "\n";
    return 0;
}
