#include <complex>
#include <deque>
#include <format>
#include <iostream>
#include <limits>
#include <map>
#include <print>
#include <queue>
#include <ranges>
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

using queue_t = tuple<pos, int>;

struct ram {
    plane<char> pl;
    plane<int> path2, dist2;
    vector<pos> blocks;
    pos start{ 0, 0 }, end{ gx, gy };
    map<int, string> which;

    ram(istream &is) : pl(xmax, ymax, '.'), path2(xmax, ymax, numeric_limits<int>::max()), dist2(xmax, ymax, 0) {
        for (const auto [v, s] : read_lines(is) | vs::enumerate) {
            auto block = split<int>(s, ",");
            blocks.emplace_back(block.front(), block.back());
            which[v] = s;
            path2.set(blocks.back(), v);
        }
    }

    optional<int> walk(int stop = ms) {
        reset(stop);
        history visited;
        for (deque<queue_t> q{ { start, 0 } }; q.size(); q.pop_front()) {
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
        auto cmp = [](auto &lhs, auto &rhs) { return get<1>(lhs) < get<1>(rhs); };
        priority_queue<queue_t, deque<queue_t>, decltype(cmp)> q(cmp);
        q.emplace(start, path2.get(start).value());
        for (; !q.empty(); q.pop()) {
            const auto [p, score] = q.top();
            if (p == end) { return which[score]; }
            for (auto npos : add(p, cardinal)) {
                int nscore = min(score, path2.get(npos).value_or(0));
                if (nscore > dist2.get(npos)) {
                    dist2.set(npos, nscore);
                    q.emplace(npos, nscore);
                }
            }
        }
        return "";
    }
};

int main(int argc, char *argv[]) {
    ram r(cin);
    println("Part1: {}", r.part1());
    println("Part2: {}", r.part2());
    return 0;
}
