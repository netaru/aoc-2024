#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "2dgrid.h"

using strings = std::vector<std::string>;

std::string sort(std::string s) {
    std::sort(s.begin(), s.end());
    return s;
}

struct station {
    map                   m;
    std::vector<position> as, xs,
            directions{ { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 } };

    station(std::istream &is) {
        std::string s;
        for (int i = 0; std::getline(is, s); ++i) {
            for (int j = 0; j < s.size(); ++j) {
                position p{ i, j };
                m[p] = s[j];
                if (s[j] == 'A') { as.push_back(p); }
                if (s[j] == 'X') { xs.push_back(p); }
            }
        }
    }

    std::string slice(position from, position delta, size_t sz) {
        std::string s;
        for (int i = 0; i < sz; ++i, from += delta) { s.push_back(m[from]); }
        return s;
    }

    int part1() {
        return std::accumulate(xs.begin(), xs.end(), 0, [this](int acc, position p) {
            strings ss =
                    std::accumulate(directions.begin(), directions.end(), strings{}, [&](strings acc, position dir) {
                        acc.push_back(slice(p, dir, 4));
                        return acc;
                    });
            return std::count_if(ss.begin(), ss.end(), [](std::string s) { return s == "XMAS"; }) + acc;
        });
    }

    int part2() {
        return std::count_if(as.begin(), as.end(), [&](position p) {
            return sort(slice(p + position{ -1, -1 }, { 1, 1 }, 3)) == "AMS" &&
                   sort(slice(p + position{ 1, -1 }, { -1, 1 }, 3)) == "AMS";
        });
    }
};

int main(int argc, char *argv[]) {
    station station(std::cin);
    std::cout << "Part1: " << station.part1() << "\n";
    std::cout << "Part2: " << station.part2() << "\n";
    return 0;
}
