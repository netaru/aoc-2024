#include <algorithm>
#include <array>
#include <cstddef>
#include <deque>
#include <format>
#include <iostream>
#include <limits>
#include <print>
#include <queue>
#include <vector>

#include "util.h"

using namespace std;

struct memory {
    i64 id, offset, size;

    memory &move(memory &other) {
        offset = other.offset;
        other.offset += size;
        other.size -= size;
        return other;
    }
};

auto cmp = [](const auto &lhs, const auto &rhs) { return lhs.offset > rhs.offset; };
using pq = priority_queue<memory, deque<memory>, decltype(cmp)>;

struct disk {
    deque<memory> occupied;
    array<pq, 10> available;

    disk(istream &is) {
        string s = read_lines(cin)[0];
        for (size_t u = 0, offset = 0; u < s.size(); u++) {
            size_t size = s[u] - '0';
            if (size == 0) continue;
            if (u % 2 == 0) {
                occupied.emplace_back(u / 2, offset, size);
            } else {
                available[size].emplace(u / 2, offset, size);
            }
            offset += size;
        }
    }

    deque<memory> fragment() {
        deque<memory> result;
        for (const auto m : occupied) {
            for (size_t u = 0; u < m.size; u++) { result.emplace_back(m.id, m.offset + u, 1); }
        }
        return result;
    }

    optional<memory> find(auto &heap, size_t size, size_t coffset) {
        size_t best = 0, offset = numeric_limits<size_t>::max();
        for (size_t u = size; u < heap.size(); u++) {
            if (!heap[u].size()) continue;
            auto noffset = heap[u].top().offset;
            if (noffset < offset and noffset < coffset) {
                best = u;
                offset = noffset;
            }
        }
        if (best == 0) return {};
        memory m = heap[best].top();
        heap[best].pop();
        return m;
    }

    void push(auto &heap, memory m) {
        if (!m.size) return;
        heap[m.size].emplace(m);
    }

    template <int part>
    i64 solve() {
        auto files = part == 1 ? fragment() : occupied;
        auto heap = available;
        auto fn = [&](memory &file) {
            if (auto m = find(heap, file.size, file.offset); m.has_value()) { push(heap, file.move(m.value())); }
            return file.id * (file.offset * file.size + (file.size * (file.size - 1)) / 2);
        };
        return rs::fold_left(files | vs::reverse | vs::transform(fn), 0l, plus());
    }
};

int main(int argc, char *argv[]) {
    disk d(cin);
    println("Part1: {}", d.solve<1>());
    println("Part2: {}", d.solve<2>());
    return 0;
}
