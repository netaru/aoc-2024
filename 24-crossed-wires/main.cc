#include <algorithm>
#include <format>
#include <iostream>
#include <map>
#include <print>
#include <ranges>
#include <tuple>
#include <utility>
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

vector<string> debug(bits_t bits, vector<wire_t> wires) {
    optional<string> carry, ncarry, sum1, sum2, carry1, carry2;
    vector<string> swapped;
    auto swap_push = [&](auto &v, auto &x) {
        swap(v, x);
        swapped.push_back(v.value_or("ERROR"));
        swapped.push_back(x.value_or("ERROR"));
    };
    auto is_z = [](auto v) { return v.value_or("t")[0] == 'z'; };
    auto correct = [&](auto ta, auto tb, auto top) -> optional<string> {
        for (const auto [reg1, op, reg2, out] : wires) {
            if (((reg1 == ta and reg2 == tb) or (reg1 == tb and reg2 == ta)) and op == top) return out;
        }
        return {};
    };
    i64 sum = rs::count_if(wires, [](auto v) { return get<3>(v)[0] == 'z'; }) - 1;
    for (i64 i = 0; i < sum; ++i) {
        sum1 = correct(format("x{:02}", i), format("y{:02}", i), "XOR");
        carry1 = correct(format("x{:02}", i), format("y{:02}", i), "AND");
        if (carry.has_value()) {
            if (carry2 = correct(carry, sum1, "AND"); !carry2.has_value()) {
                swap_push(carry1, sum1);
                carry2 = correct(carry, sum1, "AND");
            }
            sum2 = correct(carry, sum1, "XOR");
            if (sum1.has_value() and is_z(sum1)) { swap_push(sum1, sum2); }
            if (carry1.has_value() and is_z(carry1)) { swap_push(carry1, sum2); }
            if (carry2.has_value() and is_z(carry2)) { swap_push(carry2, sum2); }
            if (ncarry = correct(carry2, carry1, "OR"); is_z(ncarry) and ncarry != format("z{:02}", sum)) {
                swap_push(ncarry, sum2);
            }
            carry = ncarry;
        } else {
            carry = carry1;
        }
    }
    rs::sort(swapped);
    return swapped;
}

int main(int argc, char *argv[]) {
    bits_t bits;
    vector<wire_t> wires;

    auto input = split(read(cin), "\n\n");
    for (const string &s : split(input[0], "\n")) {
        auto row = split(s, ": ");
        bits[row[0]] = stoi(row[1]);
    }
    for (const string &s : split(input[1], "\n")) {
        auto row = split(s);
        wires.emplace_back(row[0], row[1], row[2], row[4]);
    }
    println("Part1: {}", run(bits, wires));
    println("Part2: {}", join(debug(bits, wires), ","));
    return 0;
}
