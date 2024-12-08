#pragma once

#include <complex>
#include <cstdint>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using position = std::complex<int64_t>;
using history  = std::unordered_set<position>;

template <>
struct std::hash<position> {
    std::size_t operator()(const position& p) const {
        return std::hash<int64_t>()(p.real()) ^ (std::hash<int64_t>()(p.imag()) << 1);
    }
};

struct visit {
    position where, direction;

    visit(position w, position d) : where(w), direction(d) {}
    friend bool operator==(const visit& lhs, const visit& rhs) {
        return lhs.where == rhs.where and lhs.direction == rhs.direction;
    }
};

template <>
struct std::hash<visit> {
    std::size_t operator()(const struct visit& v) const {
        return std::hash<int64_t>()(v.where.real()) ^ (std::hash<int64_t>()(v.where.imag()) << 1) ^
               std::hash<int64_t>()(v.direction.real() << 2) ^ (std::hash<int64_t>()(v.direction.imag()) << 3);
    }
};

using visit_history = std::unordered_set<visit>;

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

inline int64_t abs(position p) { return std::abs(static_cast<std::complex<double>>(p)); }
inline int64_t arg(position p) { return std::arg(static_cast<std::complex<double>>(p)); }
