#include <algorithm>
#include <complex>
#include <cstddef>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <print>
#include <queue>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

using pos     = complex<int>;
using queue_t = tuple<size_t, pos, pos>;
struct queue_less {
    bool operator()(const queue_t &lhs, const queue_t &rhs) { return get<0>(lhs) > get<0>(rhs); }
};
using set_t     = pair<pos, pos>;
using visited_t = pair<set_t, size_t>;
using pq        = priority_queue<queue_t, deque<queue_t>, queue_less>;

template <>
struct std::hash<pos> {
    std::size_t operator()(const pos &p) const {
        return std::hash<int64_t>()(p.real()) ^ (std::hash<int64_t>()(p.imag()) << 1);
    }
};

template <>
struct std::hash<set_t> {
    std::size_t operator()(const set_t &s) const {
        return std::hash<int64_t>()(s.first.real()) ^ (std::hash<int64_t>()(s.first.imag()) << 1) ^
               (std::hash<int64_t>()(s.second.imag()) << 2) ^ (std::hash<int64_t>()(s.second.imag()) << 3);
    }
};

auto pop(auto &q) {
    auto p = q.front();
    q.pop_front();
    return p;
}

struct maze {
    vector<string>               grid;
    pos                          start, end;
    unordered_map<set_t, size_t> visited;

    maze(istream &is) {
        string s;
        while (getline(is, s)) { grid.push_back(s); }
        start = locate('S');
        end   = locate('E');
        traverse();
    }

    char get(pos at) { return grid[at.imag()][at.real()]; }
    pos  locate(char ch) {
        for (int y = 0; y < grid.size(); ++y) {
            for (int x = 0; x < grid[0].size(); ++x) {
                if (grid[y][x] == ch) return { x, y };
            }
        }
        return { 0, 0 };
    }

    void traverse() {
        pq q;
        q.push({ 0, start, pos{ 1, 0 } });
        for (; !q.empty(); q.pop()) {
            auto [score, current, dir] = q.top();

            set_t key = { current, dir };
            if (visited.contains(key) and visited[key] < score) { continue; }
            visited[key] = score;

            for (auto qt : vector<queue_t>{ { score + 1, current + dir, dir },
                                            { score + 1000, current, pos{ -dir.imag(), dir.real() } },
                                            { score + 1000, current, pos{ dir.imag(), -dir.real() } } }) {
                auto [nscore, ncurrent, ndir] = qt;
                if (get(ncurrent) == '#') continue;
                q.push(qt);
            }
        }
    }

    size_t trace_back(unordered_set<set_t> target) {
        unordered_set<pos> seen;
        deque<set_t>       q{ target.begin(), target.end() };
        while (q.size()) {
            set_t key = pop(q);

            const auto [current, dir] = key;
            seen.insert(current);
            vector<tuple<set_t, size_t>> newkeys{ { { current - dir, dir }, 1 },
                                                  { { current, pos{ -dir.imag(), dir.real() } }, 1000 },
                                                  { { current, pos{ dir.imag(), -dir.real() } }, 1000 } };
            for (auto const [newkey, score] : newkeys) {
                if (visited.contains(newkey) and visited[newkey] + score == visited[key]) q.push_back(newkey);
            }
        }
        return seen.size();
    }

    size_t part1() {
        size_t m = numeric_limits<size_t>::max();
        for (visited_t vt : visited) {
            if (vt.first.first == end) { m = min(m, vt.second); }
        }
        return m;
    }

    int part2() {
        size_t               target = part1();
        unordered_set<set_t> from;
        for (visited_t vt : visited) {
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
