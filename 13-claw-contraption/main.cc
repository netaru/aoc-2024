#include <cmath>
#include <iostream>

#include "util.h"

using namespace std;

template <int part>
i64 score(double ax, double ay, double bx, double by, double xtarget, double ytarget) {
    if constexpr (part == 2) {
        xtarget += 10000000000000;
        ytarget += 10000000000000;
    }
    double ca = (xtarget * by - ytarget * bx) / (ax * by - ay * bx);
    double cb = (xtarget - ax * ca) / bx;
    if (modf(ca, &ax) == 0 and modf(cb, &ax) == 0) { return ca * 3 + cb; }
    return 0;
}

i64 solve(const auto &c, auto func) {
    auto fn = [&](auto v) { return func(v[0], v[1], v[2], v[3], v[4], v[5]); };
    return rs::fold_left(c | vs::transform(fn), 0l, plus());
}

int main(int argc, char *argv[]) {
    auto chunks = ints<double>(read(cin)) | vs::chunk(6) | rs::to<vector>();
    println("Part1: {}", solve(chunks, score<1>));
    println("Part2: {}", solve(chunks, score<2>));
    return 0;
}
