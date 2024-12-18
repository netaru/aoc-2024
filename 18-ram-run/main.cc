#include <complex>
#include <deque>
#include <format>
#include <iostream>
#include <print>
#include <tuple>
#include <unordered_set>
#include <vector>
#include "split.h"

using namespace std;

#if 1
const int gx = 70, gy = 70;
const int ms = 1024;
#else
const int gx = 6, gy = 6;
const int ms = 21;
#endif
const int xmax = gx + 1, ymax = gx + 1;

using pos   = complex<int>;
using set_t = std::unordered_set<pos>;
using queue = std::deque<std::tuple<pos, int>>;

template <>
struct std::hash<pos> {
    std::size_t operator()(const pos &p) const {
        return std::hash<int64_t>()(p.real()) ^ (std::hash<int64_t>()(p.imag()) << 1);
    }
};

queue::value_type pop(queue &q) {
    queue::value_type value = q.front();
    q.pop_front();
    return value;
}

struct ram {
    vector<string> grid;
    vector<pos>    blocks;
    pos            start, end;

    ram(istream &is) : grid(ymax), start{ 0, 0 }, end{ gx, gy } {
        for (auto &row : grid) { row = string(xmax, '.'); }
        string s;
        while (getline(is, s)) {
            auto block = split<int>(s, ',');
            blocks.emplace_back(block.front(), block.back());
        }
    }

    bool valid(pos p) {
        return p.real() >= 0 and p.real() < grid.back().size() and p.imag() >= 0 and p.imag() < grid.size();
    }
    void set(pos p, char c) {
        if (valid(p)) grid[p.imag()][p.real()] = c;
    }
    char get(pos p) { return valid(p) ? grid[p.imag()][p.real()] : '#'; }

    optional<int> walk(int stop = ms) {
        reset(stop);
        queue q{ { start, 0 } };
        set_t visited;
        while (q.size()) {
            const auto [current, step] = pop(q);
            if (current == end) { return step; }
            for (pos dir : vector<pos>{ { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } }) {
                pos npos = current + dir;
                if (get(npos) == '#' or visited.contains(npos)) continue;
                visited.insert(npos);
                q.emplace_back(npos, step + 1);
            }
        }
        return {};
    }

    void reset(int stop) {
        for (auto &row : grid) { row = string(xmax, '.'); }
        for (int u = 0; u < stop; ++u) { set(blocks[u], '#'); }
    }

    int part1() { return walk(ms).value(); }

    string part2() {
        int low = 0, high = blocks.size() - 1;
        while (low < high) {
            int mid = ((low + high) / 2);
            if (auto v = walk(mid + 1); !v.has_value()) {
                high = mid;
            } else {
                low = mid + 1;
            }
        }
        auto b = blocks[low];
        return format("{},{}", b.real(), b.imag());
    }
};

int main(int argc, char *argv[]) {
    ram r(cin);
    print("Part1: {}\n", r.part1());
    print("Part2: {}\n", r.part2());
    return 0;
}
