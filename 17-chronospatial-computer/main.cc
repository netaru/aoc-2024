#include <cmath>
#include <cstdint>
#include <iostream>
#include <optional>
#include <print>
#include <vector>

#include "util.h"

using namespace std;

struct computer {
    int64_t a, b, c, pc;
    vector<int> ops;

    computer(istream &is) : b(0), c(0) {
        auto input = split(read(is), "\n\n");
        a = ints(input[0])[0];
        ops = ints(input[1]);
    }

    int64_t combo(int64_t operand) {
        switch (operand) {
            case 3: return operand;
            case 4: return a;
            case 5: return b;
            case 6: return c;
        }
        return -1;
    }

    void reset(optional<int64_t> _a) {
        pc = 0;
        if (!_a.has_value()) return;
        a = *_a;
        b = c = 0;
    }

    template <int part = 1>
    auto run(optional<int64_t> _a = {}) {
        reset(_a);
        vector<int64_t> output;
        while (pc < ops.size()) {
            int64_t opcode = ops[pc], operand = ops[pc + 1];
            pc += 2;
            if (opcode == 0) {
                a = a >> combo(operand);
            } else if (opcode == 1) {
                b = (b ^ operand);
            } else if (opcode == 2) {
                b = combo(operand) & 7;
            } else if (opcode == 3) {
                if (a) pc = operand;
            } else if (opcode == 4) {
                b = (b ^ c);
            } else if (opcode == 5) {
                if constexpr (part == 2) return (combo(operand) % 8);
                output.push_back((combo(operand) % 8));
            } else if (opcode == 6) {
                b = a >> combo(operand);
            } else if (opcode == 7) {
                c = a >> combo(operand);
            }
        }
        if constexpr (part == 1) return output;
        if constexpr (part == 2) return 0l;
    }

    optional<int64_t> part2(auto current, int64_t value = 0) {
        if (current == ops.crend()) { return value; }
        vector<int> possible;
        for (int i = 0; i < 8; i++) {
            if (run<2>(value << 3 | i) == *current) { possible.push_back(i); }
        }
        for (int maybe : possible) {
            if (auto v = part2(current + 1, value << 3 | maybe); v.has_value()) return v;
        }
        return {};
    }

    int64_t part2() { return part2(ops.crbegin()).value_or(0); }
};

int main(int argc, char *argv[]) {
    computer p(cin);
    println("Part1: {}", join(p.run(), ","));
    println("Part2: {}", p.part2());
    return 0;
}
