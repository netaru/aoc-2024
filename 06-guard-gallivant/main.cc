#include <algorithm>
#include <functional>
#include <iostream>
#include <print>
#include <utility>

#include "util.h"

using namespace std;

struct lab {
    plane    p;
    pos      start, where, delta;
    dhistory visited;
    history  hist;

    lab(istream& is) : p(is), start(*p.locate('^').begin()), where(start), delta(0, -1) { hist.insert(where); }

    template <int part>
    bool update_direction() {
        delta = pos{ -delta.imag(), delta.real() };
        if constexpr (part == 1) { return true; }
        return visited.insert({ where, delta }).second;
    }

    template <int part = 1>
    auto walk() {
        bool run = true;
        while (p.valid(where) and run) {
            if constexpr (part == 1)
                if (p.valid(where)) { hist.insert(where); }
            if (p.get(where + delta).value_or('-') == '#')
                run = update_direction<part>();
            else
                where += delta;
        }
        if constexpr (part == 1) { return hist.size(); }
    }

    bool is_stuck(pos block) {
        where = start;
        delta = pos{ 0, -1 };
        visited.clear();
        p.set(block, '#');
        walk<2>();
        p.set(block, '.');
        return p.valid(where);
    }

    int cycles() {
        hist.erase(hist.find(start));
        auto fn = bind(mem_fn(&lab::is_stuck), this, placeholders::_1);
        return count_if(hist.begin(), hist.end(), fn);
    }
};

int main(int argc, char* argv[]) {
    lab l(cin);
    println("Part1: {}", l.walk());
    println("Part2: {}", l.cycles());
    return 0;
}
