#pragma once

#include <algorithm>
#include <cctype>
#include <charconv>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <istream>
#include <iterator>
#include <optional>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>
#include <unordered_set>
#include <vector>

using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;

namespace rs = std::ranges;
namespace vs = std::views;

template <typename T = char>
auto read(std::istream &is) {
    if constexpr (std::is_same_v<T, char>) {
        return std::string{ std::istreambuf_iterator<T>(is), {} };
    } else {
        return std::vector<T>{ std::istream_iterator<T>(is), {} };
    }
}

inline std::vector<std::string> read_lines(std::istream &is) {
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(is, line)) { lines.push_back(line); }
    return lines;
}

template <typename T = int>
std::vector<T> ints(std::string_view sv) {
    std::vector<T> result;

    std::function<int(int)> end_func = isdigit;

    if constexpr (std::is_same_v<T, double> or std::is_same_v<T, float>) {
        end_func = [](int ch) -> int { return isdigit(ch) or (char) ch == '.'; };
    }

    size_t ind = 0;
    while (true) {
        auto iter = std::find_if(sv.begin() + ind, sv.end(), isdigit);
        auto end = std::find_if_not(iter, sv.end(), end_func);
        if (iter == sv.end()) break;
        if (iter != sv.begin() and *(iter - 1) == '-') { iter--; }
        std::string_view digits = sv.substr(iter - sv.begin(), end - iter);
        if (T value; std::from_chars(digits.data(), digits.data() + digits.size(), value).ec == std::errc{}) {
            result.push_back(value);
        }
        ind = end - sv.begin();
    }
    return result;
}

inline std::string_view ltrim(std::string_view sv, std::function<int(int)> fn = ::isspace) {
    return sv.substr(std::find_if_not(sv.begin(), sv.end(), fn) - sv.begin());
}

inline std::string_view rtrim(std::string_view sv, std::function<int(int)> fn = ::isspace) {
    return sv.substr(0, sv.size() - (std::find_if_not(sv.rbegin(), sv.rend(), fn) - sv.rbegin()));
}

inline std::string_view trim(std::string_view sv) { return rtrim(ltrim(sv)); }

template <typename T = std::string>
std::vector<T> split(std::string_view sv, std::string_view delimiter = " ") {
    std::vector<T> result;
    for (const auto word : vs::split(sv, delimiter)) {
        std::string_view view = trim(std::string_view{ word });
        if (view.size()) {
            if constexpr (std::is_same_v<T, std::string>) {
                result.emplace_back(view);
            } else {
                if (T value; std::from_chars(view.data(), view.data() + view.size(), value).ec == std::errc{}) {
                    result.push_back(value);
                }
            }
        }
    }
    return result;
}
template <typename T>
std::string join(const T &, std::string_view = " ");

namespace std {
inline std::string to_string(const std::vector<char> &vec) { return std::string{ vec.begin(), vec.end() }; }

template <typename T>
inline std::string to_string(const std::vector<T> &vec) {
    return join(vec, ", ");
}
}

template <typename T>
inline std::string join(T begin, T end, std::string_view delimiter = " ") {
    using type = std::iterator_traits<decltype(begin)>::value_type;
    std::string result;
    bool added = false;
    for (; begin != end; begin++) {
        if (added) result += delimiter;
        if constexpr (std::is_same_v<type, std::string>) {
            result += *begin;
        } else {
            using std::to_string;
            result += to_string(*begin);
        }
        added = true;
    }
    return result;
}

template <typename T>
std::string join(const T &values, std::string_view delimiter) {
    return join(values.cbegin(), values.cend(), delimiter);
}

using pos = std::complex<i64>;
using history = std::unordered_set<pos>;
using poses = history;
using dhistory = std::unordered_set<std::pair<pos, pos>>;

namespace dave {
const pos N{ 0, -1 }, E{ 1, 0 }, S{ 0, 1 }, W{ -1, 0 };
const pos NE{ 1, -1 }, NW{ -1, -1 }, SW{ -1, 1 }, SE{ 1, 1 };
};

const std::array<pos, 4> cardinal{ dave::N, dave::E, dave::S, dave::W };
const std::array<pos, 4> ordinal{ dave::NE, dave::NW, dave::SW, dave::SE };

constexpr int manhattan(const pos &lhs, const pos &rhs) {
    return std::abs(lhs.real() - rhs.real()) + std::abs(lhs.imag() - rhs.imag());
}

constexpr std::vector<pos> add(pos p, const auto &vec) {
    std::vector<pos> result(vec.size());
    for (size_t u = 0; u < vec.size(); u++) { result[u] = p + vec[u]; }
    return result;
}

constexpr std::vector<pos> concat_dirs() {
    std::vector<pos> result;
    result.insert(result.end(), cardinal.begin(), cardinal.end());
    result.insert(result.end(), ordinal.begin(), ordinal.end());
    return result;
}
const std::vector<pos> compass = concat_dirs();

constexpr pos clockwise(pos p) { return pos{ -p.imag(), p.real() }; }
constexpr pos cclockwise(pos p) { return pos{ p.imag(), -p.real() }; }

template <>
struct std::hash<pos> {
    std::size_t operator()(const pos &p) const {
        return std::hash<i64>()(p.real()) ^ (std::hash<i64>()(p.imag()) << 1);
    }
};

template <>
struct std::hash<std::pair<pos, pos>> {
    std::size_t operator()(const std::pair<pos, pos> &v) const {
        return std::hash<int64_t>()(v.first.real()) ^ (std::hash<int64_t>()(v.first.imag()) << 1) ^
               std::hash<int64_t>()(v.second.real() << 2) ^ (std::hash<int64_t>()(v.second.imag()) << 3);
    }
};

template <typename T = char>
struct plane {
    using row_t = std::vector<T>;
    using data_t = std::vector<row_t>;
    data_t data;

    plane() {}
    plane(const plane<T> &other) : data(other.data) {}
    plane(std::istream &is) : data(build(read_lines(is))) {}
    plane(std::string_view s) : data(build(split(s, "\n"))) {}
    plane(size_t x, size_t y, T v) : data(y, row_t(x, v)) {}

    data_t build(std::vector<std::string> lines) {
        data_t result;
        for (auto line : lines) {
            row_t vec;
            for (auto ch : line) {
                if constexpr (std::is_same_v<T, char>) {
                    vec.push_back(ch);
                } else {
                    vec.push_back(static_cast<T>(ch - '0'));
                }
            }
            result.push_back(vec);
        }
        return result;
    }

    inline bool valid(i64 x, i64 y) const { return y >= 0 and y < data.size() and x >= 0 and x < data[y].size(); }

    bool valid(pos p) const { return valid(p.real(), p.imag()); }

    std::optional<T> get(pos p) const {
        if (valid(p)) return data[p.imag()][p.real()];
        return {};
    }

    void add_row(auto row) { data.push_back({ row.cbegin(), row.cend() }); }
    std::vector<std::string> rows() { return split(as_string(), "\n"); }
    std::vector<std::string> columns() {
        std::vector<std::string> result;
        const auto r = rows();
        for (size_t x = 0; x < max_size(); ++x) {
            std::string column;
            for (size_t y = 0; y < r.size(); ++y) {
                if (valid(x, y)) column += r[y][x];
            }
            result.push_back(column);
        }
        return result;
    }

    plane &rotate_right() {
        auto xs = vs::iota(0u, max_size());
        auto ys = vs::iota(0u, data.size()) | vs::reverse;
        return transform(xs, ys);
    }

    plane &rotate_left() {
        auto xs = vs::iota(0u, max_size()) | vs::reverse;
        auto ys = vs::iota(0u, data.size());
        return transform(xs, ys);
    }

    plane &transpose() {
        auto xs = vs::iota(0u, max_size());
        auto ys = vs::iota(0u, data.size());
        return transform(xs, ys);
    }

    plane &rtranspose() {
        auto xs = vs::iota(0u, max_size()) | vs::reverse;
        auto ys = vs::iota(0u, data.size()) | vs::reverse;
        return transform(xs, ys);
    }

    void set(std::optional<pos> p, T v) {
        if (p.has_value()) set(p.value(), v);
    }
    void set(pos p, T v) {
        if (valid(p)) data[p.imag()][p.real()] = v;
    }
    void set(const auto &ps, T v) {
        rs::for_each(ps, [&](pos p) { set(p, v); });
    }
    void set(const std::vector<std::tuple<pos, T>> values) {
        for (const auto [p, value] : values) { set(p, value); }
    }

    void swap(pos p1, pos p2) {
        if (valid(p1) and valid(p2)) { std::swap(data[p1.imag()][p1.real()], data[p2.imag()][p2.real()]); }
    }

    void reset(T v) {
        for (i64 y = 0; y < data.size(); ++y) {
            for (i64 x = 0; x < data[y].size(); ++x) { data[y][x] = v; }
        }
    }

    std::string slice(pos p, pos d, size_t sz) const {
        std::string s;
        for (int i = 0; i < sz; ++i, p += d) {
            if (std::optional<T> value = get(p); value.has_value()) {
                if constexpr (std::is_same_v<T, char>) {
                    s += value.value();
                } else {
                    s += std::to_string(value.value());
                }
            }
        }
        return s;
    }

    poses line(pos from, pos to) {
        poses result;
        for (pos at = from, delta = from - to; valid(at); at += delta) { result.insert(at); }
        for (pos at = from, delta = to - from; valid(at); at += delta) { result.insert(at); }
        return result;
    }

    poses find(T v) {
        poses result;
        for (i64 y = 0; y < data.size(); ++y) {
            for (i64 x = 0; x < data[y].size(); ++x) {
                if (data[y][x] == v) result.insert(pos{ x, y });
            }
        }
        return result;
    }

    pos find_first(T v) {
        poses result;
        for (i64 y = 0; y < data.size(); ++y) {
            for (i64 x = 0; x < data[y].size(); ++x) {
                if (data[y][x] == v) return pos{ x, y };
            }
        }
        return { -1, -1 };
    }

    std::unordered_set<T> values() {
        std::unordered_set<T> cs;
        for (i64 y = 0; y < data.size(); ++y) {
            for (i64 x = 0; x < data[y].size(); ++x) {
                auto p = pos{ x, y };
                cs.insert(get(p).value());
            }
        }
        return cs;
    }

    std::string as_string() { return join(data, "\n"); }

    inline size_t max_size() {
        return rs::max(data | vs::transform([](auto v) { return v.size(); }));
    }

   private:
    inline plane<T> &transform(auto xs, auto ys) {
        data_t ndata;
        for (auto x : xs) {
            row_t nrow;
            for (auto y : ys) {
                if (valid(x, y)) nrow.push_back(data[y][x]);
            }
            ndata.push_back(nrow);
        }
        data = ndata;
        return *this;
    }
};

auto pop(auto &q) {
    auto p = q.front();
    q.pop_front();
    return p;
}

namespace dave {

template <typename T>
T sort(T c) {
    rs::sort(c);
    return c;
}

auto sort(auto &c, auto comp = std::less()) {
    std::sort(c.begin(), c.end(), comp);
    return c;
}

inline poses merge(poses lhs, poses rhs) {
    rs::copy(lhs, std::inserter(rhs, rhs.begin()));
    return rhs;
}

template <typename T>
inline std::vector<T> vmerge(std::vector<T> lhs, std::vector<T> rhs) {
    rs::copy(lhs, std::back_inserter(rhs));
    return rhs;
}

inline std::string remove(std::string s, char c = '\n') {
    auto ret = rs::remove(s, c);
    s.erase(ret.begin(), ret.end());
    return s;
}
};
