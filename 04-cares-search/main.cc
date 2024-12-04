#include <algorithm>
#include <complex>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using position = std::complex<int64_t>;

std::string sort(std::string s) {
    std::sort(s.begin(), s.end());
    return s;
}

struct station {
    std::vector<std::string> grid;
    std::vector<position>    as, xs,
            dirs{ { -1, -1 }, { 1, 1 }, { 1, -1 }, { -1, 1 }, { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };

    station(std::istream &is) {
        std::string s;
        for (int i = 0; std::getline(is, s); ++i) {
            grid.push_back(s);
            for (int j = 0; j < s.size(); ++j) {
                if (s[j] == 'A') { as.push_back({ i, j }); }
                if (s[j] == 'X') { xs.push_back({ i, j }); }
            }
        }
    }

    bool valid(position p) {
        return p.real() >= 0 and p.real() < grid.back().size() and p.imag() >= 0 and p.imag() < grid.size();
    }

    char get(position p) { return valid(p) ? grid[p.real()][p.imag()] : '-'; }

    std::string slice(position from, position delta, size_t sz) {
        std::string s;
        for (int i = 0; i < sz; ++i, from += delta) { s += get(from); }
        return s;
    }

    int part1() {
        return std::accumulate(xs.begin(), xs.end(), 0, [this](int acc, position p) {
            return acc + std::count_if(dirs.begin(), dirs.end(), [&](position delta) {
                       return slice(p, delta, 4) == "XMAS";
                   });
        });
    }

    bool isAMS(position p, position delta) { return sort(slice(p, delta, 3)) == "AMS"; }
    int  part2() {
        return std::count_if(as.begin(), as.end(), [&](position p) {
            return isAMS(p + dirs[0], dirs[1]) and isAMS(p + dirs[2], dirs[3]);
        });
    }
};

int main(int argc, char *argv[]) {
    station station(std::cin);
    std::cout << "Part1: " << station.part1() << "\n";
    std::cout << "Part2: " << station.part2() << "\n";
    return 0;
}
