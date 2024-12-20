#include <complex>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <print>
#include <tuple>
#include <unordered_map>
#include <vector>

using namespace std;
using pos     = complex<int64_t>;
using path_t  = tuple<pos, size_t>;
using queue_t = deque<path_t>;
using nodes_t = unordered_map<pos, int64_t>;

template <>
struct std::hash<pos> {
    std::size_t operator()(const pos &p) const {
        return std::hash<int64_t>()(p.real()) ^ (std::hash<int64_t>()(p.imag()) << 1);
    }
};

auto pop(auto &q) {
    auto p = q.front();
    q.pop_front();
    return p;
}

constexpr int64_t get_distance(const pos &lhs, const pos &rhs) {
    return abs(lhs.real() - rhs.real()) + abs(lhs.imag() - rhs.imag());
}

struct cpu {
    nodes_t        nodes;
    vector<string> grid;
    pos            start, end;

    cpu(istream &is) {
        string  s;
        int64_t y = 0;
        while (getline(is, s)) {
            grid.push_back(s);
            for (int64_t x = 0; x < s.size(); ++x) {
                if (s[x] == 'S') start = pos{ x, y };
                if (s[x] == 'E') end = pos{ x, y };
            }
            y++;
        }
        traverse();
    }

    bool valid(pos p) {
        return p.real() >= 0 and p.real() < grid.back().size() and p.imag() >= 0 and p.imag() < grid.size();
    }
    char get(pos p) { return valid(p) ? grid[p.imag()][p.real()] : '#'; }

    void traverse() {
        queue_t q{ { start, 0 } };
        nodes.emplace(start, 0);
        while (q.size()) {
            const auto [current, ms] = pop(q);
            for (pos dir : vector<pos>{ { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } }) {
                pos npos = current + dir;
                if (nodes.contains(npos) or get(npos) == '#') continue;
                nodes.emplace(npos, ms + 1);
                q.emplace_back(npos, ms + 1);
            }
        }
    }

    int64_t cheat(int64_t cutoff, int64_t max_distance) {
        int64_t score = 0;
        for (auto iter1 = nodes.cbegin(), iter2 = iter1; iter1 != nodes.cend(); iter1++, iter2 = iter1) {
            for (iter2++; iter2 != nodes.cend(); iter2++) {
                const auto [pos1, cost1] = *iter1;
                const auto [pos2, cost2] = *iter2;

                int64_t distance = get_distance(pos1, pos2);
                if (distance <= max_distance and abs(cost1 - cost2) >= cutoff + distance) score++;
            }
        }
        return score;
    }
};

int main(int argc, char *argv[]) {
    cpu c(cin);
    print("Part1: {}\n", c.cheat(100, 2));
    print("Part2: {}\n", c.cheat(100, 20));
    return 0;
}
