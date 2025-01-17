#include <iostream>
#include <map>
#include <print>
#include <ranges>
#include <tuple>
#include <vector>

#include "util.h"

using namespace std;

using bits_t = map<string, i64>;
using wire_t = tuple<string, string, string, string>;

i64 read_reg(bits_t bits, char which) {
    i64 shift = 0, ret = 0;
    for (const auto [key, value] : bits | vs::filter([which](const auto v) { return get<0>(v)[0] == which; }))
        ret |= value << shift++;
    return ret;
}

i64 run(bits_t bits, vector<wire_t> wires) {
    while (!wires.empty()) {
        vector<wire_t> nwires;
        for (const auto t : wires) {
            const auto [reg1, op, reg2, out] = t;
            if (bits.contains(reg1) and bits.contains(reg2)) {
                if (op == "XOR") {
                    bits[out] = bits[reg1] ^ bits[reg2];
                } else if (op == "OR") {
                    bits[out] = bits[reg1] | bits[reg2];
                } else if (op == "AND") {
                    bits[out] = bits[reg1] & bits[reg2];
                }
            } else {
                nwires.push_back(t);
            }
        }
        wires = nwires;
    }

    return read_reg(bits, 'z');
}

int main(int argc, char *argv[]) {
    auto           input = split(read(cin), "\n\n");
    bits_t         bits;
    vector<wire_t> wires;
    for (const string &s : split(input[0], "\n")) {
        auto row     = split(s, ": ");
        bits[row[0]] = stoi(row[1]);
    }
    for (const string &s : split(input[1], "\n")) {
        auto row = split(s);
        wires.emplace_back(row[0], row[1], row[2], row[4]);
    }
    println("Part1: {}", run(bits, wires));
    println("But it should be: {}", read_reg(bits, 'x') + read_reg(bits, 'y'));
    return 0;
}
