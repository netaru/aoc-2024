#include <complex>
#include <cstdint>
#include <deque>
#include <iostream>
#include <limits>
#include <numeric>
#include <print>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "util.h"

using namespace std;

using queue_t = tuple<pos, string>;

using cache_key_t = tuple<pos, pos, int64_t>;
using cache_t     = unordered_map<cache_key_t, int64_t>;

template <>
struct std::hash<cache_key_t> {
    std::size_t operator()(const cache_key_t &ck) const {
        const auto [p1, p2, num] = ck;
        return std::hash<pos>()(p1) ^ (std::hash<pos>()(p2) << 16) ^ (std::hash<int64_t>()(num) << 32);
    }
};

const vector<pos> directions{ { 1, 0 }, { -1, 0 }, { 0, -1 }, { 0, -1 } };

cache_t cache;
struct dirpad {
    const unordered_map<char, pos> locations{
        { '^', { 1, 0 } }, { 'A', { 2, 0 } }, { '<', { 0, 1 } }, { 'v', { 1, 1 } }, { '>', { 2, 1 } },
    };
    const unordered_set<pos> valid{ { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } };

    const pos start{ 2, 0 };

    int64_t cheapest(pos from, pos to, int64_t n) {
        cache_key_t ck{ from, to, n };
        if (cache.contains(ck)) return cache[ck];
        int64_t        result = numeric_limits<int64_t>::max();
        deque<queue_t> q{ { from, "" } };
        while (q.size()) {
            const auto [cur, presses] = pop(q);
            if (!valid.contains(cur)) continue;
            if (cur == to) {
                int64_t score = solve(presses + 'A', n - 1);
                result        = min(result, score);
                continue;
            }
            if (cur.real() < to.real()) {
                q.emplace_back(cur + pos{ 1, 0 }, presses + '>');
            } else if (cur.real() > to.real()) {
                q.emplace_back(cur + pos{ -1, 0 }, presses + '<');
            }
            if (cur.imag() < to.imag()) {
                q.emplace_back(cur + pos{ 0, 1 }, presses + 'v');
            } else if (cur.imag() > to.imag()) {
                q.emplace_back(cur + pos{ 0, -1 }, presses + '^');
            }
        }
        cache[ck] = result;
        return result;
    }

    int64_t solve(const std::string &s, int n = 1) {
        if (n == 0) return s.size();
        int64_t result = 0;
        pos     cur    = start;
        for (const auto c : s) {
            pos next = locations.at(c);
            result += cheapest(cur, next, n);
            cur = next;
        }
        return result;
    }
};

struct numpad {
    const unordered_map<char, pos> locations{ { '7', { 0, 0 } }, { '8', { 1, 0 } }, { '9', { 2, 0 } },
                                              { '4', { 0, 1 } }, { '5', { 1, 1 } }, { '6', { 2, 1 } },
                                              { '1', { 0, 2 } }, { '2', { 1, 2 } }, { '3', { 2, 2 } },
                                              { '0', { 1, 3 } }, { 'A', { 2, 3 } } };
    const unordered_set<pos>       valid{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 },
                                          { 0, 2 }, { 1, 2 }, { 2, 2 }, { 1, 3 }, { 2, 3 } };

    const pos start{ 2, 3 };

    dirpad dp;

    int64_t cheapest(pos from, pos to, int n) {
        int64_t        result = numeric_limits<int64_t>::max();
        deque<queue_t> q{ { from, "" } };
        while (q.size()) {
            const auto [cur, presses] = pop(q);
            if (!valid.contains(cur)) continue;
            if (cur == to) {
                int64_t score = dp.solve(presses + 'A', n);
                result        = min(result, score);
                continue;
            }
            if (cur.real() < to.real()) {
                q.emplace_back(cur + pos{ 1, 0 }, presses + '>');
            } else if (cur.real() > to.real()) {
                q.emplace_back(cur + pos{ -1, 0 }, presses + '<');
            }
            if (cur.imag() < to.imag()) {
                q.emplace_back(cur + pos{ 0, 1 }, presses + 'v');
            } else if (cur.imag() > to.imag()) {
                q.emplace_back(cur + pos{ 0, -1 }, presses + '^');
            }
        }
        return result;
    }

    int64_t solve(string_view sv, int n = 2) {
        int64_t result = 0;
        pos     cur    = start;
        for (const auto c : sv) {
            pos next = locations.at(c);
            result += cheapest(cur, next, n);
            cur = next;
        }
        return result;
    }

    int64_t score(vector<string> ss, int n = 2) {
        return accumulate(ss.begin(), ss.end(), 0l, [&](int64_t acc, string s) {
            return acc + solve(s, n) * ints<int64_t>(s).front();
        });
    }
};

int main(int argc, char *argv[]) {
    numpad         np;
    string         s;
    vector<string> ss;
    while (getline(cin, s)) { ss.push_back(s); }
    print("Part1: {}\n", np.score(ss, 2));
    print("Part2: {}\n", np.score(ss, 25));
    return 0;
}
