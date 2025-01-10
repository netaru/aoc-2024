#include <algorithm>
#include <complex>
#include <cstddef>
#include <deque>
#include <iostream>
#include <limits>
#include <print>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include "util.h"

using namespace std;

using queue_t = tuple<pair<pos, pos>, size_t>;
struct queue_less {
    bool operator()(const queue_t &lhs, const queue_t &rhs) { return get<1>(lhs) > get<1>(rhs); }
};
using set_t     = pair<pos, pos>;
using visited_t = unordered_map<set_t, size_t>;
using pq        = priority_queue<queue_t, deque<queue_t>, queue_less>;

struct maze {
    plane     pl;
    pos       start, end;
    visited_t visited;

    maze(istream &is) : pl(is), start(*pl.find('S').begin()), end(*pl.find('E').begin()) { traverse(); }

    void traverse() {
        pq q;
        q.push({ { start, pos{ 1, 0 } }, 0 });
        for (; !q.empty(); q.pop()) {
            auto [key, score]   = q.top();
            auto [current, dir] = key;
            if (visited.contains(key) and visited[key] < score) { continue; }
            visited[key] = score;

            for (auto qt : vector<queue_t>{ { { current + dir, dir }, score + 1 },
                                            { { current, clockwise(dir) }, score + 1000 },
                                            { { current, cclockwise(dir) }, score + 1000 } }) {
                auto [p2, nscore]     = qt;
                auto [ncurrent, ndir] = p2;
                if (pl.get(ncurrent) == '#') continue;
                q.push(qt);
            }
        }
    }

    size_t trace_back(unordered_set<set_t> target) {
        history seen;
        for (deque<set_t> q{ target.begin(), target.end() }; q.size(); q.pop_front()) {
            set_t key = q.front();

            const auto [current, dir] = key;
            seen.insert(current);
            vector<tuple<set_t, size_t>> newkeys{ { { current - dir, dir }, 1 },
                                                  { { current, clockwise(dir) }, 1000 },
                                                  { { current, cclockwise(dir) }, 1000 } };
            for (auto const [newkey, score] : newkeys) {
                if (visited.contains(newkey) and visited[newkey] + score == visited[key]) q.push_back(newkey);
            }
        }
        return seen.size();
    }

    size_t part1() {
        size_t m = numeric_limits<size_t>::max();
        for (auto vt : visited) {
            if (vt.first.first == end) { m = min(m, vt.second); }
        }
        return m;
    }

    int part2() {
        size_t               target = part1();
        unordered_set<set_t> from;
        for (auto vt : visited) {
            if (vt.first.first == end and vt.second == target) { from.insert(vt.first); }
        }
        return trace_back(from);
    }
};

int main(int argc, char *argv[]) {
    maze m(cin);
    print("Part1: {}\n", m.part1());
    print("Part2: {}\n", m.part2());
    return 0;
}
