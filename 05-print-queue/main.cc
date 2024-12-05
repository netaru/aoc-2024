#include <functional>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "split.h"

using page_t  = std::vector<int>;
using rules_t = std::unordered_map<int, std::unordered_set<int>>;

int middle(page_t p) { return p[(p.size() / 2)]; }

struct printer {
    rules_t                       rules;
    std::vector<page_t>           pages;
    std::function<bool(int, int)> comp;

    printer(std::istream &is) {
        std::string s;
        while (std::getline(is, s)) {
            if (s.contains("|")) {
                std::vector<int> rule = split<int>(s, '|');
                rules[rule.front()].insert(rule.back());
            } else if (s.size()) {
                pages.push_back(split<int>(s, ','));
            }
        }
        comp = [this](int lhs, int rhs) { return rules[lhs].contains(rhs); };
    }

    bool   is_sorted(page_t p) { return std::is_sorted(p.begin(), p.end(), comp); }
    page_t sort(page_t p) {
        std::sort(p.begin(), p.end(), comp);
        return p;
    }

    int part1() {
        return std::accumulate(
                pages.begin(), pages.end(), 0, [&](int acc, page_t p) { return acc + (is_sorted(p) ? middle(p) : 0); });
    }

    int part2() {
        return std::accumulate(pages.begin(), pages.end(), 0, [&](int acc, page_t p) {
            return acc + (!is_sorted(p) ? middle(sort(p)) : 0);
        });
    }
};

int main(int argc, char *argv[]) {
    printer p(std::cin);
    std::cout << "Part1: " << p.part1() << "\n";
    std::cout << "Part2: " << p.part2() << "\n";
    return 0;
}
