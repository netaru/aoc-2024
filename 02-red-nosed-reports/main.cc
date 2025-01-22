#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <ranges>
#include <tuple>
#include <vector>

#include "util.h"

using namespace std;

using report = vector<int>;
using reports = vector<report>;

bool is_sorted(report r) { return is_sorted(r.begin(), r.end()) or is_sorted(r.begin(), r.end(), greater<int>()); }

bool element_difference_check(report r) {
    auto view = r | vs::adjacent<2>;
    auto it = rs::find_if(view, [](tuple<int, int> p) {
        int value = abs(get<0>(p) - get<1>(p));
        return (value > 3 or value == 0);
    });
    return it == view.end();
}

bool part1_predicate(report r) { return is_sorted(r) and element_difference_check(r); }

bool part2_predicate(report r) {
    bool safe = part1_predicate(r);
    if (!safe) {
        for (int i = 0; i < r.size(); i++) {
            report tmp = r;
            tmp.erase(tmp.begin() + i);
            if (safe = part1_predicate(tmp); safe) break;
        }
    }
    return safe;
}

reports parse() {
    reports rs;
    for (const auto s : read_lines(cin)) rs.push_back(split<int>(s));
    return rs;
}

int main(int argc, char *argv[]) {
    reports rs = parse();
    cout << "Part1: " << rs::count_if(rs, part1_predicate) << "\n";
    cout << "Part2: " << rs::count_if(rs, part2_predicate) << "\n";
    return 0;
}
