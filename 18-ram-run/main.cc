#include <complex>
#include <deque>
#include <format>
#include <iostream>
#include <print>
#include <tuple>
#include <vector>

#include "util.h"

using namespace std;

#if 1
const int gx = 70, gy = 70;
const int ms = 1024;
#else
const int gx = 6, gy = 6;
const int ms = 21;
#endif
const int xmax = gx + 1, ymax = gx + 1;

using queue = deque<tuple<pos, int>>;

struct ram {
    plane       pl;
    vector<pos> blocks;
    pos         start, end;

    ram(istream &is) : pl(xmax, ymax, '.'), start{ 0, 0 }, end{ gx, gy } {
        for (auto &s : read_lines(is)) {
            auto block = split<int>(s, ",");
            blocks.emplace_back(block.front(), block.back());
        }
    }

    optional<int> walk(int stop = ms) {
        reset(stop);
        history visited;
        for (queue q{ { start, 0 } }; q.size(); q.pop_front()) {
            const auto [current, step] = q.front();
            if (current == end) { return step; }
            for (pos npos : add(current, cardinal)) {
                if (pl.get(npos).value_or('#') == '#' or visited.contains(npos)) continue;
                visited.insert(npos);
                q.emplace_back(npos, step + 1);
            }
        }
        return {};
    }

    void reset(int stop) {
        pl.reset('.');
        for (int u = 0; u < stop; ++u) { pl.set(blocks[u], '#'); }
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
    println("Part1: {}", r.part1());
    println("Part2: {}", r.part2());
    return 0;
}
