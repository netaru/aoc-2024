#include <algorithm>
#include <climits>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <print>
#include <span>
#include <string>
#include <vector>
#include "split.h"

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
    int x, y, dx, dy;

    robot(int _x, int _y, int _dx, int _dy) : x(_x), y(_y), dx(_dx), dy(_dy) {}

    void tick() {
        x += dx;
        y += dy;
        if (x < 0) { x += x_size; }
        if (x >= x_size) { x %= x_size; }
        if (y < 0) { y += y_size; }
        if (y >= y_size) { y %= y_size; }
    }

    pair<int, bool> quadrant() {
        if (x >= 0 and x < half_x and y >= 0 and y < half_y) return { 0, true };
        if (x > half_x and x < x_size and y >= 0 and y < half_y) return { 1, true };
        if (x >= 0 and x < half_x and y > half_y and y < y_size) return { 2, true };
        if (x >= 0 and x > half_x and y > half_y and y < y_size) return { 3, true };
        return { 0, false };
    }
};

int product(const vector<robot> robots) {
    array<int, 4> q{ 0, 0, 0, 0 };
    for (auto r : robots) {
        if (auto [quad, valid] = r.quadrant(); valid) { q[quad]++; }
    }
    return accumulate(q.begin(), q.end(), 1, multiplies());
}

vector<robot> parse(istream& is) {
    vector<robot> robots;
    string        s;
    while (getline(is, s)) {
        auto line  = split(s);
        auto pos   = split<int>(line[0].substr(2), ',');
        auto delta = split<int>(line[1].substr(2), ',');
        robots.emplace_back(pos[0], pos[1], delta[0], delta[1]);
    }
    return robots;
}

void print_grid(span<robot> robots) {
    vector<string> grid(y_size);
    for (auto& s : grid) { s = string(x_size, '.'); }
    for (const auto r : robots) { grid[r.y][r.x] = '#'; }
    copy(grid.begin(), grid.end(), ostream_iterator<string>(cout, "\n"));
}

int main(int argc, char* argv[]) {
    int  part1, min_product = numeric_limits<int>::max(), min_iteration = 0;
    auto robots = parse(cin);
    for (int i = 1; i < 10000; ++i) {
        for_each(robots.begin(), robots.end(), [](robot& r) { r.tick(); });
        int current = product(robots);
        if (current < min_product) {
            min_product   = current;
            min_iteration = i;
            print("New min product at iteration {}\n", i);
            print_grid(robots);
        }
        if (i == 100) { part1 = current; }
    }
    print("Part1: {}\n", part1);
    print("Part2: {}\n", min_iteration);
    return 0;
}
