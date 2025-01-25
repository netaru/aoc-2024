#include <complex>
#include <deque>
#include <iostream>
#include <optional>
#include <print>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "util.h"

using namespace std;

const unordered_map<char, pos> dir{ { '<', dave::W }, { '>', dave::E }, { '^', dave::N }, { 'v', dave::S } };

struct warehouse {
    vector<string> in;
    plane<char> grid, init;
    pos robot;
    string input;

    warehouse(istream &is) : in(split(read(is), "\n\n")), grid(in[0]), init(grid), input(dave::remove(in[1], '\n')) {}

    optional<pos> find_next_available(pos from, pos delta) {
        while (true) {
            from += delta;
            if (grid.get(from) == '.') { return { from }; }
            if (grid.get(from) == '#') { return {}; }
        }
        return {};
    }

    bool is_block(pos at) { return (grid.get(at) == ']' or grid.get(at) == '['); }
    bool is_horizontal(pos delta) { return delta == dave::W or delta == dave::E; }

    bool move_horizontal(pos from, pos delta) {
        deque<pair<pos, pos>> moves;

        pos prev = from;
        for (from += delta; is_block(from); prev = from, from += delta) { moves.emplace_front(from, prev); }

        if (grid.get(from) == '.') {
            moves.emplace_front(from, prev);
            for (auto &[opos, npos] : moves) { grid.swap(opos, npos); }
            return true;
        }
        return false;
    }

    optional<pair<pos, pos>> get_block(pos at) {
        if (grid.get(at) == ']') return { { at + dave::W, at } };
        if (grid.get(at) == '[') return { { at, at + dave::E } };
        return {};
    }

    bool move_vertical(pos from, pos delta) {
        deque<pair<pos, pos>> moves;
        unordered_set<pos> current{ from }, next;
        bool done = false;
        while (!done) {
            done = true;
            for (auto v : current) {
                pos npos = v + delta;
                moves.emplace_front(v, npos);
                if (grid.get(npos) == '#') { return false; }
                if (auto block = get_block(npos); block.has_value()) {
                    done = false;
                    next.insert(block->first);
                    next.insert(block->second);
                }
            }
            current = next;
            next.clear();
        }
        for (auto &[opos, npos] : moves) { grid.swap(opos, npos); }
        return true;
    }

    int part1() {
        robot = grid.find_first('@');
        for (const auto c : input) {
            pos delta = dir.at(c), npos = robot + delta;
            if (grid.get(npos) == '.') {
                grid.swap(robot, npos);
                robot = npos;
            } else if (grid.get(npos) == 'O') {
                if (auto next = find_next_available(npos, delta); next.has_value()) {
                    grid.swap(npos, *next);
                    grid.swap(robot, npos);
                    robot = npos;
                }
            }
        }
        return score('O');
    }

    int part2() {
        upscale();
        robot = grid.find_first('@');
        for (const auto c : input) {
            pos delta = dir.at(c), npos = robot + delta;
            if (grid.get(npos) == '.') {
                grid.swap(robot, npos);
                robot = npos;
            } else if (is_block(npos) and is_horizontal(delta)) {
                if (move_horizontal(robot, delta)) { robot = npos; }
            } else if (is_block(npos) and !is_horizontal(delta)) {
                if (move_vertical(robot, delta)) { robot = npos; }
            }
        }
        return score('[');
    }

    void upscale() {
        grid.data.clear();
        for (auto row : init.rows()) {
            auto fn = [](auto c) { return c == '@' ? "@."s : c == 'O' ? "[]"s : string(2, c); };
            auto nrow = row | vs::transform(fn) | rs::to<vector<string>>();
            grid.add_row(join(nrow, ""));
        }
    }

    i64 score(char ch) {
        return rs::fold_left(
                grid.find(ch) | vs::transform([](auto p) { return p.imag() * 100 + p.real(); }), 0l, plus());
    }
};

int main(int argc, char *argv[]) {
    warehouse wh(cin);
    println("Part1: {}", wh.part1());
    println("Part2: {}", wh.part2());
    return 0;
}
