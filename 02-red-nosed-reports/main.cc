#include <iostream>

#include "util.h"

using namespace std;

using report = vector<int>;
using reports = vector<report>;

bool is_sorted(span<int> r) { return rs::is_sorted(r) or rs::is_sorted(r | vs::reverse); }

bool element_difference_check(span<int> r) {
    auto view = r | vs::adjacent<2>;
    auto it = rs::find_if(view, [](auto t) {
        const auto [first, second] = t;
        int value = abs(first - second);
        return (value > 3 or value == 0);
    });
    return it == view.end();
}

bool part1_predicate(span<int> r) { return is_sorted(r) and element_difference_check(r); }

bool part2_predicate(span<int> r) {
    bool safe = part1_predicate(r);
    if (!safe) {
        for (int i = 0; i < r.size(); i++) {
            report tmp{ r.begin(), r.end() };
            tmp.erase(tmp.begin() + i);
            if (safe = part1_predicate(tmp); safe) break;
        }
    }
    return safe;
}

int main(int argc, char *argv[]) {
    reports rs = read_lines(cin) | vs::transform([](auto s) { return split<int>(s); }) | rs::to<reports>();
    println("Part1: {}", rs::count_if(rs, part1_predicate));
    println("Part2: {}", rs::count_if(rs, part2_predicate));
    return 0;
}
