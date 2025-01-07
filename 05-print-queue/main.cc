#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <set>
#include <vector>
#include "util.h"

using namespace std;
constexpr int middle(const auto &p) { return p[(p.size() / 2)]; }

struct printer {
    map<int, set<int>>       rules;
    vector<vector<int>>      pages;
    function<bool(int, int)> comp;

    printer(istream &is) : comp([this](int lhs, int rhs) { return rules[lhs].contains(rhs); }) {
        auto input = split(read(is), "\n\n");
        for (const auto &rule : split(input[0], "\n") | views::transform([](auto s) { return split<int>(s, "|"); }))
            rules[rule.front()].insert(rule.back());
        for (const auto &s : split(input[1], "\n")) { pages.push_back(split<int>(s, ",")); }
    }

    bool is_sorted(const auto &p) const { return std::is_sorted(p.begin(), p.end(), comp); }
    auto sort(auto p) {
        std::sort(p.begin(), p.end(), comp);
        return p;
    }

    template <int part>
    int solve() {
        function<int(span<int>)> fn;
        if constexpr (part == 1)
            fn = [&](auto p) { return is_sorted(p) ? middle(p) : 0; };
        else
            fn = [&](auto p) { return is_sorted(p) ? 0 : middle(sort(p)); };
        return transform_reduce(pages.begin(), pages.end(), 0, plus(), fn);
    }
};

int main(int argc, char *argv[]) {
    printer p(cin);
    cout << "Part1: " << p.solve<1>() << "\n";
    cout << "Part2: " << p.solve<2>() << "\n";
    return 0;
}
