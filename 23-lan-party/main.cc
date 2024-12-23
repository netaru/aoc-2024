#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <istream>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <unordered_map>

using namespace std;

using network_t = set<string>;
using nodes_t   = unordered_map<string, set<string>>;

template <>
struct std::hash<network_t> {
    std::size_t operator()(const network_t &ss) const {
        std::size_t       hash;
        std::hash<string> hasher;
        for (string s : ss) { hash = hash ^ hasher(s); }
        return hash;
    }
};

struct LAN {
    nodes_t nodes;
    LAN(istream &is) {
        string s;
        while (getline(is, s)) {
            string first = s.substr(0, 2), second = s.substr(3, 2);
            nodes[first].insert(second);
            nodes[second].insert(first);
        }
    }

    uint64_t part1() {
        set<network_t> networks;
        for (const auto x : views::keys(nodes)) {
            for (const auto y : nodes[x]) {
                for (const auto z : nodes[y]) {
                    if (x != z and nodes[z].contains(x)) {
                        network_t network{ x, y, z };
                        if (any_of(network.begin(), network.end(), [](string_view s) { return s[0] == 't'; })) {
                            networks.insert(network);
                        }
                    }
                }
            }
        }
        return networks.size();
    }
};

int main(int argc, char *argv[]) {
    LAN l(cin);
    print("Part1: {}\n", l.part1());
    return 0;
}
