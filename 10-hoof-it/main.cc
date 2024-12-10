#include <deque>
#include <iostream>
#include <istream>
#include <string>
#include <utility>
#include <vector>

#include "2dgrid.h"
using map_t = std::unordered_map<position, int>;

struct top {
    map_t                 pos;
    set                   starts;
    position              up{ 1, 0 }, down{ -1, 0 }, right{ 0, 1 }, left{ 0, -1 };
    std::vector<position> dirs{ up, down, right, left };

    top(std::istream &is) {
        std::string s;
        int         y = 0;
        while (std::getline(is, s)) {
            for (int x = 0; x < s.size(); x++) {
                position current{ x, y };
                if (s[x] == '0') { starts.insert(current); }
                if (s[x] != '.') pos[current] = (s[x] - '0');
            }
            ++y;
        }
    }

    int score() {
        int sum = 0;
        for (position p : starts) {
            int                                  count = 0;
            std::deque<std::pair<position, int>> q{ { p, 0 } };
            set                                  visited;
            // std::cout << "New Iter: " << p << "\n";
            while (q.size()) {
                const auto &[curr, value] = q.front();
                // std::cout << "Here: " << curr << ", value: " << pos[curr] << ", input: " << value << "\n";
                q.pop_front();
                if (visited.contains(curr)) continue;
                visited.insert(curr);
                if (value == 9) {
                    count++;
                    continue;
                }

                int seek = value + 1;
                for (auto dir : dirs) {
                    position new_pos = curr + dir;
                    if (pos[new_pos] == seek and !visited.contains(new_pos)) { q.push_back({ new_pos, seek }); }
                }
            }
            // std::cout << "With a sum of: " << count << "\n";
            sum += count;
        }
        return sum;
    }
};

int main(int argc, char *argv[]) {
    std::cout << "This is the result\n";
    top t(std::cin);
    std::cout << "Part1: " << t.score() << "\n";
    return 0;
}
