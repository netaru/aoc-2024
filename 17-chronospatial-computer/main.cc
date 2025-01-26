#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <iostream>
#include <iterator>
#include <numeric>
#include <optional>
#include <ostream>
#include <print>
#include <string>
#include <vector>
#include "split.h"

using namespace std;

struct computer {
    int64_t           a, b, c, pc;
    array<int64_t, 3> init;
    vector<int>       ops;

    computer(istream &is) {
        string s;
        int    inx = 0;
        while (getline(is, s)) {
            if (!s.size()) continue;
            auto sp = split(s);
            if (sp.front() == "Register") {
                init[inx++] = stoi(sp.back());
            } else if (sp.front() == "Program:") {
                ops = split<int>(sp.back(), ',');
            }
        }
        a = init[0];
        b = init[1];
        c = init[2];
    }

    int combo(int operand) {
        switch (operand) {
            case 0:
            case 1:
            case 2:
            case 3: return operand;
            case 4: return a;
            case 5: return b;
            case 6: return c;
        }
        return 1;
    }

    void reset(optional<int64_t> _a) {
        pc = 0;
        if (!_a.has_value()) return;
        a = *_a;
        b = c = 0;
    }

    vector<int> run(optional<int64_t> _a = {}, bool once = false) {
        reset(_a);
        vector<int> output;
        while (pc < ops.size()) {
            int opcode = ops[pc], operand = ops[pc + 1];
            pc += 2;
            if (opcode == 0) {
                a = a >> combo(operand);
            } else if (opcode == 1) {
                b = (b ^ operand);
            } else if (opcode == 2) {
                b = (combo(operand) % 8);
            } else if (opcode == 3) {
                if (a) pc = operand;
            } else if (opcode == 4) {
                b = (b ^ c) & 0x7;
            } else if (opcode == 5) {
                output.push_back((combo(operand) & 7));
                if (once) { break; }
            } else if (opcode == 6) {
                b = a >> combo(operand);
            } else if (opcode == 7) {
                c = a >> combo(operand);
            }
        }
        return output;
    }

    optional<int64_t> part2(auto current, int64_t value = 0) {
        if (current == ops.crend()) return value;
        vector<int> possible;
        for (int i = 0; i < 8; i++) {
            if (run(value << 3 | i, true).front() == *current) { possible.push_back(i); }
        }
        for (int maybe : possible) {
            if (auto v = part2(current + 1, value << 3 | maybe); v.has_value()) return v;
        }
        return {};
    }

    int64_t part2() { return part2(ops.crbegin()).value_or(0); }
};

string to_string(vector<int> out) {
    string s;
    bool   added = false;
    for (auto i : out) {
        s += (added ? "," : "") + to_string(i);
        added = true;
    }
    return s;
}

int main(int argc, char *argv[]) {
    computer p(cin);
    cout << "Part1: " + to_string(p.run()) + "\n";
    cout << "Part2: " << p.part2() << "\n";
    return 0;
}
