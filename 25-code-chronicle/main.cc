#include <algorithm>
#include <array>
#include <iostream>
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
            (pl.get(pos{ 0, 0 }) == '.' ? keys : locks).push_back(item);
        }
    }

    int part1() {
        auto fn = [&](auto lock) {
            return rs::count_if(keys, [&](auto key) {
                return rs::none_of(
                        vs::zip_transform([](int l, int k) { return l + k > 7; }, lock, key), [](bool b) { return b; });
            });
        };
        return rs::fold_left(locks | vs::transform(fn), 0, plus());
    }
};

int main(int argc, char *argv[]) {
    code c(cin);
    println("Part1: {}", c.part1());
    return 0;
}
