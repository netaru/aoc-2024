#include <array>
#include <iostream>
#include <istream>
#include <print>
#include <vector>

using namespace std;

using grid    = vector<string>;
using tumbler = array<int, 5>;

struct code {
    vector<tumbler> keys, locks;

    code(istream &is) {
        string s;
        grid   current;
        while (getline(is, s)) {
            if (!s.size()) {
                if (current.size()) { parse(current); }
                current.clear();
            } else {
                current.push_back(s);
            }
        }
        if (current.size()) { parse(current); }
    }

    bool is_key(const grid g) { return g[0][0] == '.'; }

    void parse(const grid g) {
        tumbler item;
        for (auto &v : item) v = 0;
        for (int x = 0; x < g[0].size(); ++x) {
            for (int y = 0; y < g.size(); ++y) {
                if (g[y][x] == '#') item[x]++;
            }
        }
        if (is_key(g)) {
            keys.push_back(item);
        } else {
            locks.push_back(item);
        }
    }

    bool fits(tumbler lock, tumbler key) {
        for (int i = 0; i < lock.size(); i++) {
            if (lock[i] + key[i] > 7) return false;
        }
        return true;
    }

    int part1() {
        int result = 0;
        for (auto l : locks) {
            for (auto k : keys) {
                if (fits(l, k)) { result++; }
            }
        }
        return result;
    }
};

int main(int argc, char *argv[]) {
    code c(cin);
    print("Part1: {}\n", c.part1());
    return 0;
}
