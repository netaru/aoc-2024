#include <algorithm>
#include <complex>
#include <deque>
#include <iostream>
#include <iterator>
#include <print>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

using pos = complex<int>;

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

using step = tuple<pos, int, int, unordered_set<pos>>;

struct maze {
    vector<string> grid;
    pos            start, end;
    vector<pos>    dirs{ { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };

    maze(istream &is) {
        string s;
        while (getline(is, s)) { grid.push_back(s); }
        start = locate('S');
        end   = locate('E');
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

    int turn_left(int dir) {
        dir--;
        if (dir < 0) dir += dirs.size();
        return dir;
    }

    int turn_right(int dir) {
        dir++;
        return dir % dirs.size();
    }

    int part1() {
        unordered_map<pos, int> visited;

        deque<step> paths{ { start, 0, 0, {} } };
        while (paths.size()) {
            auto [current, dir, score, history] = pop(paths);
            if (get(current) == '#') continue;
            if (visited.contains(current) and visited[current] <= score) { continue; }
            visited[current] = score;

            int left = turn_left(dir), right = turn_right(dir);
            paths.push_back({ current + dirs[left], left, score + 1001, history });
            paths.push_back({ current + dirs[dir], dir, score + 1, history });
            paths.push_back({ current + dirs[right], right, score + 1001, history });
        }

        return visited[end];
    }

    // 7036
    // 11048
    // 101492
    int part2() {
        unordered_map<pos, int> visited;
        unordered_set<pos>      best;

        deque<step> paths{ { start, 0, 0, {} } };
        while (paths.size()) {
            auto [current, dir, score, history] = pop(paths);
            if (get(current) == '#') continue;
            if (visited.contains(current) and visited[current] * 2 < score) { continue; }
            if (!visited.contains(current)) {
                visited[current] = score;
            } else if (score < visited[current]) {
                visited[current] = score;
            }
            history.insert(current);
            if (current == end and score == 101492) {
                copy(history.begin(), history.end(), inserter(best, best.begin()));
            }

            int left = turn_left(dir), right = turn_right(dir);
            paths.push_back({ current + dirs[left], left, score + 1001, history });
            paths.push_back({ current + dirs[dir], dir, score + 1, history });
            paths.push_back({ current + dirs[right], right, score + 1001, history });
        }

        print_grid(best);

        return best.size();
    }

    void print_grid(unordered_set<pos> paths) {
        auto cp = grid;
        for (auto p : paths) { cp[p.imag()][p.real()] = 'O'; }
        copy(cp.begin(), cp.end(), ostream_iterator<string>(cout, "\n"));
    }
};

int main(int argc, char *argv[]) {
    maze m(cin);
    print("Part1: {}\n", m.part1());
    print("Part2: {}\n", m.part2());
    return 0;
}
