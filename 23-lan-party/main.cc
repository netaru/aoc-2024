#include <algorithm>
#include <cstdint>
#include <iostream>
#include <istream>
#include <iterator>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <unordered_map>
#include "util.h"

using namespace std;

struct LAN {
    unordered_map<string, set<string>> nodes;

    set<set<string>> connected;

    LAN(istream &is) {
        set<string> keys;
        for (const auto &s : read_lines(is)) {
            string first = s.substr(0, 2), second = s.substr(3, 2);
            nodes[first].insert(second);
            nodes[second].insert(first);
            keys.insert(first);
            keys.insert(second);
        }
        bkb({}, keys, {});
    }

    void bkb(set<string> R, set<string> P, set<string> X) {
        if (P.empty() and X.empty()) {
            connected.insert(R);
        } else {
            set<string> nn;
            if (!P.empty()) {
                auto &v = nodes[*P.begin()];
                rs::set_difference(P, v, inserter(nn, nn.begin()));
            }

            for (const string &s : nn) {
                R.insert(s);
                const auto &v = nodes[s];
                set<string> newP, newX;
                rs::set_intersection(P, v, inserter(newP, newP.begin()));
                rs::set_intersection(X, v, inserter(newP, newP.begin()));

                bkb(R, newP, newX);
                R.erase(s);
                P.erase(s);
                X.insert(s);
            }
        }
    }

    uint64_t part1() {
        set<set<string>> p1;
        for (auto x : vs::keys(nodes)) {
            for (auto y : nodes[x]) {
                for (auto z : nodes[y]) {
                    if (z != x and nodes[z].contains(x)) p1.insert({ x, y, z });
                }
            }
        }
        return rs::count_if(p1, [&](auto s) { return rs::any_of(s, [&](auto c) { return c[0] == 't'; }); });
    }

    string part2() {
        auto max = rs::max_element(connected, [](auto lhs, auto rhs) { return lhs.size() < rhs.size(); });
        return join(*max, ",");
    }
};

int main(int argc, char *argv[]) {
    LAN l(cin);
    println("Part1: {}", l.part1());
    println("Part2: {}", l.part2());
    return 0;
}
