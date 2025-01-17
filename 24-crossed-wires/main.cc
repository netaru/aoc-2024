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
    vector<string> swapped;
    auto           push    = [&](auto v) { swapped.push_back(v.value_or("ERROR")); };
    auto           returns = [&](auto ta, auto tb, auto top) -> optional<string> {
        for (const auto [reg1, op, reg2, out] : wires) {
            if (((reg1 == ta and reg2 == tb) or (reg1 == tb and reg2 == ta)) and op == top) return out;
        }
        return {};
    };
    optional<string> carry = {}, new_carry = {}, sum1, sum2, carry1, carry2;

    i64 sum = rs::count_if(wires, [](auto v) { return get<3>(v)[0] == 'z'; }) - 1;
    for (i64 i = 0; i < sum; ++i) {
        auto xreg = format("x{:02}", i);
        auto yreg = format("y{:02}", i);

        sum1   = returns(xreg, yreg, "XOR");
        carry1 = returns(xreg, yreg, "AND");

        if (carry.has_value()) {
            carry2 = returns(carry, sum1, "AND");
            if (!carry2.has_value()) {
                swap(carry1, sum1);
                push(carry1);
                push(sum1);
                carry2 = returns(carry, sum1, "AND");
            }
            sum2 = returns(carry, sum1, "XOR");
            if (sum1.has_value() and sum1.value()[0] == 'z') {
                swap(sum1, sum2);
                push(sum1);
                push(sum2);
            }
            if (carry1.has_value() and carry1.value()[0] == 'z') {
                swap(carry1, sum2);
                push(carry1);
                push(sum2);
            }
            if (carry2.has_value() and carry2.value()[0] == 'z') {
                swap(carry2, sum2);
                push(carry2);
                push(sum2);
            }
            new_carry = returns(carry2, carry1, "OR");
        } else {
            new_carry = {};
        }

        if (new_carry.has_value() and new_carry.value()[0] == 'z' and new_carry.value() != format("z{:02}", sum)) {
            swap(new_carry, sum2);
            push(new_carry);
            push(sum2);
        }

        if (carry.has_value()) {
            carry = new_carry;
        } else {
            carry = carry1;
        }
    }
    rs::sort(swapped);
    return swapped;
}

int main(int argc, char *argv[]) {
    bits_t         bits;
    vector<wire_t> wires;

    auto input = split(read(cin), "\n\n");
    for (const string &s : split(input[0], "\n")) {
        auto row     = split(s, ": ");
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
