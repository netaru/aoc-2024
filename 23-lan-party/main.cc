#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <istream>
#include <iterator>
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

    set<network_t> connected;

    LAN(istream &is) {
        string s;
        while (getline(is, s)) {
            string first = s.substr(0, 2), second = s.substr(3, 2);
            nodes[first].insert(second);
            nodes[second].insert(first);
        }
        for (const auto node : views::keys(nodes)) { connect(node, { node }); }
    }

    void connect(string node, network_t req) {
        if (connected.contains(req)) return;
        connected.insert(req);
        for (const auto n : nodes[node]) {
            if (req.contains(n)) continue;
            network_t nreq;
            set_intersection(req.begin(), req.end(), nodes[n].begin(), nodes[n].end(), inserter(nreq, nreq.begin()));
            if (nreq.size() != req.size()) continue;
            nreq.insert(n);
            connect(n, nreq);
        }
    }

    uint64_t part1() {
        uint64_t total = 0;
        for (auto s : connected) {
            if (s.size() == 3 and any_of(s.begin(), s.end(), [](string_view sv) { return sv[0] == 't'; })) total += 1;
        }
        return total;
    }

    string part2() {
        auto   max   = max_element(connected.begin(), connected.end(), [](network_t lhs, network_t rhs) {
            return lhs.size() < rhs.size();
        });
        bool   added = false;
        string result;
        for (auto s : *max) {
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
