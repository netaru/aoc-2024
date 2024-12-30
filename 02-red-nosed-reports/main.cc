#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <ranges>
#include <string>
#include <tuple>
#include <vector>

#include "util.h"

using namespace std;

using report  = vector<int>;
using reports = vector<report>;

bool is_sorted(report r) { return is_sorted(r.begin(), r.end()) or is_sorted(r.begin(), r.end(), greater<int>()); }

bool element_difference_check(report r) {
    auto view = r | std::views::adjacent<2>;
    auto it   = ranges::find_if(view, [](tuple<int, int> p) {
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
    string  s;
    while (getline(cin, s)) rs.push_back(split<int>(s));
    return rs;
}

int main(int argc, char *argv[]) {
    reports rs = parse();
    cout << "Part1: " << count_if(rs.begin(), rs.end(), part1_predicate) << "\n";
    cout << "Part2: " << count_if(rs.begin(), rs.end(), part2_predicate) << "\n";
    return 0;
}
