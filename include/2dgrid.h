#pragma once

#include <complex>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using position = std::complex<int64_t>;

template <>
struct std::hash<position> {
    std::size_t operator()(const position& p) const {
        return std::hash<int64_t>()(p.real()) ^ (std::hash<int64_t>()(p.imag()) << 1);
    }
};

using map      = std::unordered_map<position, char>;
using map_pair = std::pair<position, char>;
using set      = std::unordered_set<position>;

inline int64_t norm(int64_t value) {
    if (value >= 1) return 1;
    if (value <= -1) return -1;
    return 0;
}

inline position correction(position head, position tail) {
    position n = head - tail;
    return { norm(n.real()), norm(n.imag()) };
}
