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

using network_t = vector<int>;
using nodes_t   = unordered_map<int, network_t>;

struct LAN {
    nodes_t nodes;

    set<network_t> connected;

    int    id(string s) { return s[0] | s[1] << 8; }
    string name(int i) { return { char(i & 0xff), char(i >> 8 & 0xff) }; }

    LAN(istream &is) {
        string s;
        while (getline(is, s)) {
            int first = id(s.substr(0, 2)), second = id(s.substr(3, 2));
            nodes[first].push_back(second);
            nodes[second].push_back(first);
        }
        for (auto &[key, value] : nodes) { sort(value.begin(), value.end()); }
        for (const auto node : views::keys(nodes)) { connect(node, { node }); }
    }

    void connect(int node, network_t req) {
        if (connected.contains(req)) return;
        connected.insert(req);
        for (const auto n : nodes[node]) {
            if (binary_search(req.begin(), req.end(), n)) continue;
            network_t nreq;
            set_intersection(req.begin(), req.end(), nodes[n].begin(), nodes[n].end(), inserter(nreq, nreq.begin()));
            if (nreq.size() != req.size()) continue;
            nreq.push_back(n);
            sort(nreq.begin(), nreq.end());
            connect(n, nreq);
        }
    }

    uint64_t part1() {
        uint64_t total = 0;
        for (auto s : connected) {
            if (s.size() == 3 and any_of(s.begin(), s.end(), [&](int i) { return name(i)[0] == 't'; })) total += 1;
        }
        return total;
    }

    string part2() {
        auto   max   = max_element(connected.begin(), connected.end(), [](network_t lhs, network_t rhs) {
            return lhs.size() < rhs.size();
        });
        bool   added = false;
        string result;
        for (auto i : *max) {
            if (added) result += ",";
            result += name(i);
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
