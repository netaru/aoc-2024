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
#include <vector>

using namespace std;

using network_t = set<int>;
using nodes_t   = unordered_map<int, network_t>;

struct LAN {
    nodes_t nodes;

    set<network_t> connected;

    int    id(string s) { return s[0] | s[1] << 8; }
    string name(int i) { return { char(i & 0xff), char(i >> 8 & 0xff) }; }

    LAN(istream &is) {
        string   s;
        set<int> keys;
        while (getline(is, s)) {
            int first = id(s.substr(0, 2)), second = id(s.substr(3, 2));
            nodes[first].insert(second);
            nodes[second].insert(first);
            keys.insert(first);
            keys.insert(second);
        }
        bkb({}, keys, {});
    }

    void bkb(set<int> R, set<int> P, set<int> X) {
        if (P.empty() and X.empty()) {
            connected.insert(R);
        } else {
            set<int> nn;
            if (!P.empty()) {
                auto &v = nodes[*P.begin()];
                set_difference(P.cbegin(), P.cend(), v.cbegin(), v.cend(), inserter(nn, nn.begin()));
            }

            for (int i : nn) {
                R.insert(i);
                auto    &v = nodes[i];
                set<int> newP;
                set_intersection(P.begin(), P.end(), v.begin(), v.end(), inserter(newP, newP.begin()));
                set<int> newX;
                set_intersection(X.begin(), X.end(), v.begin(), v.end(), inserter(newX, newX.begin()));
                bkb(R, newP, newX);

                R.erase(i);
                P.erase(i);
                X.insert(i);
            }
        }
    }

    uint64_t part1() {
        set<set<int>> p1;
        for (auto x : views::keys(nodes)) {
            for (auto y : nodes[x]) {
                for (auto z : nodes[y]) {
                    if (z != x and nodes[z].contains(x)) p1.insert({ x, y, z });
                }
            }
        }
        return count_if(p1.begin(), p1.end(), [&](set<int> s) {
            return any_of(s.begin(), s.end(), [&](int i) { return name(i)[0] == 't'; });
        });
    }

    string part2() {
        auto max = max_element(connected.begin(), connected.end(), [](network_t lhs, network_t rhs) {
            return lhs.size() < rhs.size();
        });

        vector<string> vec;
        for (auto i : *max) { vec.push_back(name(i)); }

        bool   added = false;
        string result;
        sort(vec.begin(), vec.end());
        for (auto s : vec) {
            if (added) result += ",";
            result += s;
            added = true;
        }
        return result;
    }
};

int main(int argc, char *argv[]) {
    LAN l(cin);
    print("Part1: {}\n", l.part1());
    print("Part2: {}\n", l.part2());
    return 0;
}
