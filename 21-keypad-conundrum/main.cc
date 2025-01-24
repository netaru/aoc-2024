#include <complex>
#include <iostream>
#include <limits>
#include <print>
#include <queue>
#include <ranges>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "util.h"

using namespace std;

using cache_key_t = tuple<pos, pos, i64>;
using cache_t = unordered_map<cache_key_t, i64>;

template <>
struct std::hash<cache_key_t> {
    std::size_t operator()(const cache_key_t &ck) const {
        const auto [p1, p2, num] = ck;
        return std::hash<pos>()(p1) ^ (std::hash<pos>()(p2) << 16) ^ (std::hash<i64>()(num) << 32);
    }
};

struct dirpad {
    plane<char> pad;
    cache_t cache;

    dirpad() {
        pad.add_row("-^A"sv);
        pad.add_row("<v>"sv);
    }

    i64 cheapest(pos from, pos to, i64 n) {
        cache_key_t ck{ from, to, n };
        if (cache.contains(ck)) return cache[ck];
        i64 result = numeric_limits<i64>::max();
        queue<pair<pos, string>> q;
        q.emplace(from, "");
        for (; !q.empty(); q.pop()) {
            const auto [cur, presses] = q.front();
            if (cur == to) {
                i64 score = solve(presses + 'A', n - 1);
                result = min(result, score);
                continue;
            }
            if (pad.get(cur).value_or('-') == '-') continue;
            if (cur.real() < to.real()) {
                q.emplace(cur + dave::E, presses + '>');
            } else if (cur.real() > to.real()) {
                q.emplace(cur + dave::W, presses + '<');
            }
            if (cur.imag() < to.imag()) {
                q.emplace(cur + dave::S, presses + 'v');
            } else if (cur.imag() > to.imag()) {
                q.emplace(cur + dave::N, presses + '^');
            }
        }
        return cache[ck] = result;
    }

    i64 solve(string_view sv, int n = 1) {
        if (n == 0) return sv.size();
        pos cur{ 2, 0 };
        auto fn = [&](auto c) {
            pos prev = cur, next = pad.find_first(c);
            return cheapest(prev, cur = next, n);
        };
        return rs::fold_left(sv | vs::transform(fn), 0l, plus());
    }
};

struct numpad {
    plane<char> pad;
    dirpad dp;

    numpad() {
        pad.add_row("789"sv);
        pad.add_row("456"sv);
        pad.add_row("123"sv);
        pad.add_row("-0A"sv);
    }

    i64 cheapest(pos from, pos to, int n) {
        i64 result = numeric_limits<i64>::max();
        queue<pair<pos, string>> q;
        q.emplace(from, "");
        for (; !q.empty(); q.pop()) {
            const auto [cur, presses] = q.front();
            if (cur == to) {
                result = min(result, dp.solve(presses + 'A', n));
                continue;
            }
            if (pad.get(cur).value_or('-') == '-') continue;
            if (cur.real() < to.real()) {
                q.emplace(cur + dave::E, presses + '>');
            } else if (cur.real() > to.real()) {
                q.emplace(cur + dave::W, presses + '<');
            }
            if (cur.imag() < to.imag()) {
                q.emplace(cur + dave::S, presses + 'v');
            } else if (cur.imag() > to.imag()) {
                q.emplace(cur + dave::N, presses + '^');
            }
        }
        return result;
    }

    i64 solve(string_view sv, int n = 2) {
        pos cur{ 2, 3 };
        auto fn = [&](auto c) {
            pos prev = cur, next = pad.find_first(c);
            return cheapest(prev, cur = next, n);
        };
        return rs::fold_left(sv | vs::transform(fn), 0l, plus());
    }

    i64 score(vector<string> ss, int n = 2) {
        return rs::fold_left(ss | vs::transform([&](auto s) { return solve(s, n) * ints<i64>(s)[0]; }), 0l, plus());
    }
};

int main(int argc, char *argv[]) {
    numpad np;
    vector<string> ss = read_lines(cin);
    println("Part1: {}", np.score(ss, 2));
    println("Part2: {}", np.score(ss, 25));
    return 0;
}
