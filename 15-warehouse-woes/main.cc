#include <algorithm>
#include <complex>
#include <deque>
#include <iostream>
#include <iterator>
#include <optional>
#include <print>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

using pos = complex<int>;

template <>
struct std::hash<pos> {
    std::size_t operator()(const pos &p) const {
        return std::hash<int64_t>()(p.real()) ^ (std::hash<int64_t>()(p.imag()) << 1);
    }
};

struct warehouse {
    vector<string>           grid, init;
    pos                      robot, start, left{ -1, 0 }, right{ 1, 0 }, up{ 0, -1 }, down{ 0, 1 };
    string                   input;
    unordered_map<char, pos> dir{ { '<', left }, { '>', right }, { '^', up }, { 'v', down } };

    pos locate(char ch) {
        for (int y = 0; y < grid.size(); ++y) {
            for (int x = 0; x < grid[0].size(); ++x) {
                if (grid[y][x] == ch) return { x, y };
            }
        }
        return { 0, 0 };
    }

    warehouse(istream &is) {
        string s;
        while (getline(is, s)) {
            if (s == "") {
                copy(istream_iterator<char>(cin), {}, back_inserter(input));
                break;
            }
            init.push_back(s);
        }
        grid  = init;
        start = locate('@');
    }

    void reset() {
        grid  = init;
        robot = start;
    }

    void set(pos at, char ch) { grid[at.imag()][at.real()] = ch; }
    char get(pos at) { return grid[at.imag()][at.real()]; }

    void swap(pos first, pos second) {
        char c1 = get(first), c2 = get(second);
        set(first, c2);
        set(second, c1);
    }

    optional<pos> find_next_available(pos from, pos delta) {
        while (true) {
            from += delta;
            if (get(from) == '.') { return { from }; }
            if (get(from) == '#') { return {}; }
        }
        return {};
    }

    bool is_block(pos at) { return (get(at) == ']' or get(at) == '['); }
    bool is_horizontal(pos delta) { return delta == left or delta == right; }

    bool move_horizontal(pos from, pos delta) {
        deque<pair<pos, pos>> moves;

        pos prev = from;
        for (from += delta; is_block(from); prev = from, from += delta) { moves.emplace_front(from, prev); }

        if (get(from) == '.') {
            moves.emplace_front(from, prev);
            for (auto &[opos, npos] : moves) { swap(opos, npos); }
            return true;
        }
        return false;
    }

    optional<pair<pos, pos>> get_block(pos at) {
        if (get(at) == ']') return { { at + left, at } };
        if (get(at) == '[') return { { at, at + right } };
        return {};
    }

    bool move_vertical(pos from, pos delta) {
        deque<pair<pos, pos>> moves;
        unordered_set<pos>    current{ from }, next;
        bool                  done = false;
        while (!done) {
            done = true;
            for (auto v : current) {
                pos npos = v + delta;
                moves.emplace_front(v, npos);
                if (get(npos) == '#') { return false; }
                if (auto block = get_block(npos); block.has_value()) {
                    done = false;
                    next.insert(block->first);
                    next.insert(block->second);
                }
            }
            current = next;
            next.clear();
        }
        for (auto &[opos, npos] : moves) { swap(opos, npos); }
        return true;
    }

    int part1() {
        reset();
        for (const auto c : input) {
            pos delta = dir[c], npos = robot + delta;
            if (get(npos) == '.') {
                swap(robot, npos);
                robot = npos;
            } else if (get(npos) == 'O') {
                if (auto next = find_next_available(npos, delta); next.has_value()) {
                    swap(npos, *next);
                    swap(robot, npos);
                    robot = npos;
                }
            }
        }
        return score('O');
    }

    int part2() {
        reset();
        upscale();
        for (const auto c : input) {
            pos delta = dir[c], npos = robot + delta;
            if (get(npos) == '.') {
                swap(robot, npos);
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
        vector<string> ngrid(grid.size());
        for (int y = 0; y < grid.size(); y++) {
            for (int x = 0; x < grid[0].size(); x++) {
                if (grid[y][x] == '.') {
                    ngrid[y] += "..";
                } else if (grid[y][x] == '#') {
                    ngrid[y] += "##";
                } else if (grid[y][x] == '@') {
                    ngrid[y] += "@.";
                } else if (grid[y][x] == 'O') {
                    ngrid[y] += "[]";
                }
            }
        }
        grid  = ngrid;
        robot = locate('@');
    }

    int64_t score(char ch) {
        int64_t sc = 0;
        for (int y = 0; y < grid.size(); y++) {
            for (int x = 0; x < grid[0].size(); x++) {
                if (get({ x, y }) == ch) { sc += (y * 100 + x); }
            }
        }
        return sc;
    }
};

int main(int argc, char *argv[]) {
    warehouse wh(cin);
    print("Part1: {}\n", wh.part1());
    print("Part2: {}\n", wh.part2());
    return 0;
}
