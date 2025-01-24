#include <climits>
#include <cstdio>
#include <iostream>
#include <limits>
#include <optional>
#include <print>
#include <ranges>
#include <span>
#include <string>
#include <utility>
#include <vector>

#include "util.h"

using namespace std;

#if 1
const int x_size = 101;
const int y_size = 103;
#else
const int x_size = 11;
const int y_size = 7;
#endif

const int half_x = x_size / 2;
const int half_y = y_size / 2;

struct robot {
    pos p, d;

    robot(int x, int y, int dx, int dy) : p(x, y), d(dx, dy) {}

    void tick() {
        p += d;
        if (p.real() < 0) { p.real(p.real() + x_size); }
        if (p.real() >= x_size) { p.real(p.real() % x_size); }
        if (p.imag() < 0) { p.imag(p.imag() + y_size); }
        if (p.imag() >= y_size) { p.imag(p.imag() % y_size); }
    }

    optional<int> quadrant() const {
        auto x = p.real(), y = p.imag();
        if (x >= 0 and x < half_x and y >= 0 and y < half_y) return 0;
        if (x > half_x and x < x_size and y >= 0 and y < half_y) return 1;
        if (x >= 0 and x < half_x and y > half_y and y < y_size) return 2;
        if (x >= 0 and x > half_x and y > half_y and y < y_size) return 3;
        return {};
    }
};

int product(span<robot> robots) {
    array<int, 4> q{ 0, 0, 0, 0 };
    for (const auto& r : robots) {
        if (auto quad = r.quadrant(); quad.has_value()) { q[quad.value()]++; };
    }
    return rs::fold_left(q, 1, multiplies());
}

vector<robot> parse(istream& is) {
    vector<robot> robots;
    for (auto line : read_lines(is) | vs::transform([](auto s) { return split(s); })) {
        auto pos = split<int>(line[0].substr(2), ","), delta = split<int>(line[1].substr(2), ",");
        robots.emplace_back(pos[0], pos[1], delta[0], delta[1]);
    }
    return robots;
}

string picture(span<robot> robots) {
    plane p(x_size, y_size, '.');
    for (const auto& r : robots) { p.set(r.p, '#'); }
    return p.as_string();
}

void tick(vector<robot>& robots) {
    for (auto& r : robots) { r.tick(); }
}

bool operator==(const robot& r1, const robot& r2) { return r1.p == r2.p; }

int main(int argc, char* argv[]) {
    auto min = make_pair(0, numeric_limits<int>::max());
    auto init = parse(cin), robots = init;
    tick(robots);
    string last;
    for (int i = 2; init != robots; ++i) {
        tick(robots);
        int current = product(robots);
        if (current < min.second) {
            min = make_pair(i, current);
            last = picture(robots);
        }
        if (i == 100) { println("Part1: {}", current); }
    }
    println("Part2: {}", min.first);
    println("{}", last);
    return 0;
}
