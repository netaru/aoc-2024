#include <deque>
#include <iostream>
#include <istream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

#include "2dgrid.h"

using map_t = std::unordered_map<position, int>;
using queue = std::deque<std::pair<position, int>>;

queue::value_type pop(queue &q) {
    queue::value_type value = q.front();
    q.pop_front();
    return value;
}

struct top {
    map_t grid;
    set   starts;

    top(std::istream &is) {
        std::string s;
        int         y = 0;
        while (std::getline(is, s)) {
            for (int x = 0; x < s.size(); x++) {
                position current{ x, y };
                if (s[x] == '0') { starts.insert(current); }
                grid[current] = (s[x] - '0');
            }
            ++y;
        }
    }

    template <int part>
    int bfs(position pos) {
        int sum = 0;
        set visited;

        queue q{ { pos, 0 } };
        while (q.size()) {
            const auto &[at, value] = pop(q);
            if constexpr (part == 1) {
                if (visited.contains(at)) continue;
                visited.insert(at);
            }
            if (value == 9) {
                sum++;
                continue;
            }

            int seek = value + 1;
            for (auto dir : std::vector<position>{ { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } }) {
                position next = at + dir;
                if (grid[next] == seek) { q.push_back({ next, seek }); }
            }
        }
        return sum;
    }

    template <int part = 1>
    int score() {
        return std::accumulate(
                starts.begin(), starts.end(), 0, [&](int acc, position pos) { return acc + bfs<part>(pos); });
    }
};

int main(int argc, char *argv[]) {
    top t(std::cin);
    std::cout << "Part1: " << t.score() << "\n";
    std::cout << "Part1: " << t.score<2>() << "\n";
    return 0;
}
