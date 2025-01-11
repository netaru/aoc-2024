#include <cstdint>
#include <deque>
#include <iostream>
#include <optional>
#include <print>
#include <string>
#include <unordered_map>
#include <vector>

#include "util.h"

using namespace std;

struct gate {
    string            which, name;
    optional<int64_t> _left, _right, _signal;

    gate(string _which, string _name) : which(_which), name(_name) {}

    void left(int64_t sig) {
        _left = sig;
        eval();
    }

    void right(int64_t sig) {
        _right = sig;
        eval();
    }

    void eval() {
        if (is_done()) {
            int first = _left.value(), second = _right.value();
            if (which == "XOR") {
                _signal = first ^ second;
            } else if (which == "AND") {
                _signal = first & second;
            } else if (which == "OR") {
                _signal = first | second;
            }
        }
    }

    int64_t signal() { return _signal.value(); }

    bool is_done() { return (_left.has_value() and _right.has_value()); }
};

struct board {
    deque<pair<string, int>> signals;

    unordered_map<string, gate>           gates;
    unordered_map<string, vector<string>> lefts, rights;

    board(istream &is) {
        auto input = split(read(is), "\n\n");
        for (const string &s : split(input[0], "\n")) {
            vector<string> line = split(s, ":");
            signals.push_back({ line[0], stoi(line[1]) });
        }
        for (const string &s : split(input[1], "\n")) {
            vector<string> line = split(s);
            gates.insert({ line.back(), gate{ line[1], line.back() } });
            lefts[line[0]].push_back(line.back());
            rights[line[2]].push_back(line.back());
        }
    }

    void light() {
        while (!signals.empty()) {
            const auto [name, sig] = signals.front();
            signals.pop_front();

            for (const auto l : lefts[name]) {
                gate &g = gates.at(l);
                g.left(sig);
                if (g.is_done()) { signals.push_back({ l, g.signal() }); }
            }
            for (const auto r : rights[name]) {
                gate &g = gates.at(r);
                g.right(sig);
                if (g.is_done()) { signals.push_back({ r, g.signal() }); }
            }
        }
    }

    int64_t part1() {
        int64_t value = 0;
        for (const auto [n, g] : gates) {
            if (g.name[0] == 'z') { value |= (g._signal.value() << stol(g.name.substr(1))); }
        }
        return value;
    }

    void out() {
        for (const auto [n, g] : gates) {
            cout << "Gate: " << g.name << " has gate " << g.which << ", left: " << g._left.value_or(-1)
                 << ", right: " << g._right.value_or(-1) << ", signal: " << g._signal.value_or(-1) << "\n";
        }
    }
};

int main(int argc, char *argv[]) {
    board b(cin);
    b.light();
    // b.out();
    print("Part1: {}\n", b.part1());
    return 0;
}
