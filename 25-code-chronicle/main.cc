#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <print>
#include <vector>

#include "util.h"

using namespace std;

struct code {
    vector<array<int, 5>> keys, locks;

    code(istream &is) {
        for (auto &s : split(read(is), "\n\n")) {
            plane pl(s);
            array<int, 5> item = { 0 };
            for (auto p : pl.find('#')) { item[p.real()]++; }
            auto &curr = pl.get(pos{ 0, 0 }) == '.' ? keys : locks;
            curr.push_back(item);
        }
    }

    bool fits(array<int, 5> lock, array<int, 5> key) {
        for (int i = 0; i < lock.size(); i++) {
            if (lock[i] + key[i] > 7) return false;
        }
        return true;
    }

    int part1() {
        return transform_reduce(locks.begin(), locks.end(), 0, plus(), [&](auto l) {
            return count_if(keys.begin(), keys.end(), [&](auto k) { return fits(l, k); });
        });
    }
};

int main(int argc, char *argv[]) {
    code c(cin);
    println("Part1: {}", c.part1());
    return 0;
}
