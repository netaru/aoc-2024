#include <complex>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <print>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
using pos = complex<int64_t>;

using path_t  = tuple<pos, size_t>;
using saves_t = vector<int64_t>;
using queue_t = deque<path_t>;

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

int64_t get_distance(pos lhs, pos rhs) {
    int64_t real = abs(lhs.real() - rhs.real());
    int64_t imag = abs(lhs.imag() - rhs.imag());
    return real + imag;
}

struct cpu {
    vector<string> grid;
    pos            start, end;
    int64_t        goal;

    unordered_map<pos, int64_t> path;
    unordered_map<pos, int64_t> distance;

    cpu(istream &is) {
        string s;
        while (getline(is, s)) { grid.push_back(s); }
        start = locate('S');
        end   = locate('E');
        traverse();
    }
    bool valid(pos p) {
        return p.real() >= 0 and p.real() < grid.back().size() and p.imag() >= 0 and p.imag() < grid.size();
    }
    char get(pos p) { return valid(p) ? grid[p.imag()][p.real()] : '#'; }

    void traverse() {
        queue_t q{ { start, 0 } };
        path.emplace(start, 0);
        while (q.size()) {
            const auto [current, ms] = pop(q);
            for (pos dir : vector<pos>{ { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } }) {
                pos npos = current + dir;
                if (path.contains(npos) or get(npos) == '#') continue;
                path[npos] = ms + 1;
                q.emplace_back(npos, ms + 1);
            }
        }
        goal = path[end];
        for (const auto [key, value] : path) { distance.emplace(key, goal - value); }
    }

    pos locate(char ch) {
        for (int y = 0; y < grid.size(); ++y) {
            for (int x = 0; x < grid[0].size(); ++x) {
                if (grid[y][x] == ch) return { x, y };
            }
        }
        return { 0, 0 };
    }

    vector<pair<pos, int64_t>> cheat_pos(pos at, int64_t distance) {
        vector<pair<pos, int64_t>> result;
        for (const auto [current, cost] : path) {
            int64_t dist = get_distance(at, current);
            if (dist <= distance) { result.emplace_back(current, dist); }
        }
        return result;
    }

    saves_t cheat(pos at, int64_t dist) {
        saves_t better;
        int64_t current = path[at];
        for (auto const [npos, dist] : cheat_pos(at, dist)) {
            int64_t n    = current + distance[npos] + dist;
            int64_t time = goal - n;
            if (time > 0) better.push_back(time);
        }
        return better;
    }

    int64_t check(int64_t cutoff, int64_t distance) {
        int64_t score = 0;
        for (const auto [key, value] : path) {
            saves_t saves = cheat(key, distance);
            for (auto s : saves) {
                if (s >= cutoff) { score++; }
            }
        }
        return score;
    }

    int64_t part1() { return check(100, 2); }
};

int main(int argc, char *argv[]) {
    cpu c(cin);
    print("Part1: {}\n", c.check(100, 2));
    print("Part2: {}\n", c.check(100, 20));
    return 0;
}
