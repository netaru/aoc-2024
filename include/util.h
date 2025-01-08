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

using u8  = uint8_t;
using i8  = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;

template <typename T = char>
auto read(std::istream &is) {
    if constexpr (std::is_same_v<T, char>) {
        return std::string{ std::istreambuf_iterator<T>(is), {} };
    } else {
        return std::vector<T>{ std::istream_iterator<T>(is), {} };
    }
}

inline std::vector<std::string> read_lines(std::istream &is) {
    std::string              line;
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
        auto end  = std::find_if_not(iter, sv.end(), end_func);
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
    for (const auto word : std::views::split(sv, delimiter)) {
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
inline std::string join(T begin, T end, std::string_view delimiter = " ") {
    using type = std::iterator_traits<decltype(begin)>::value_type;
    std::string result;
    bool        added = false;
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
std::string join(const T &values, std::string_view delimiter = " ") {
    return join(values.cbegin(), values.cend(), delimiter);
}

using pos      = std::complex<i64>;
using history  = std::unordered_set<pos>;
using dhistory = std::unordered_set<std::pair<pos, pos>>;

const pos                N{ 0, -1 }, E{ 1, 0 }, S{ 0, 1 }, W{ -1, 0 };
const pos                NE{ 1, -1 }, NW{ -1, -1 }, SW{ -1, 1 }, SE{ 1, 1 };
const std::array<pos, 4> cardinal{ N, E, S, W };
const std::array<pos, 4> ordinal{ NE, NW, SW, SE };

constexpr std::vector<pos> concat_dirs() {
    std::vector<pos> result;
    result.insert(result.end(), cardinal.begin(), cardinal.end());
    result.insert(result.end(), ordinal.begin(), ordinal.end());
    return result;
}
const std::vector compass = concat_dirs();

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

struct plane {
    std::vector<std::string> data;

    plane(std::istream &is) { std::ranges::copy(read_lines(is), std::back_inserter(data)); }
    plane(std::string_view s) { std::ranges::copy(split(s, "\n"), std::back_inserter(data)); }
    plane(size_t x, size_t y, char ch) : data(y, std::string(x, ch)) {}

    bool valid(pos p) const {
        return p.real() >= 0 and p.real() < data.size() and p.imag() >= 0 and p.imag() < data[p.real()].size();
    }

    std::optional<char> get(pos p) const {
        if (valid(p)) return data[p.imag()][p.real()];
        return {};
    }

    void set(pos p, char c) { data[p.imag()][p.real()] = c; }

    std::string slice(pos p, pos d, size_t sz) {
        std::string s;
        for (int i = 0; i < sz; ++i, p += d) { s += get(p).value_or('-'); }
        return s;
    }

    std::vector<pos> locate(char ch) {
        std::vector<pos> result;
        for (i64 y = 0; y < data.size(); ++y) {
            for (i64 x = 0; x < data[y].size(); ++x) {
                if (auto p = pos{ x, y }; get(p).has_value() and get(p).value() == ch) result.push_back(p);
            }
        }
        return result;
    }

    std::string as_string() {
        std::ostringstream oss;
        std::ranges::copy(data, std::ostream_iterator<std::string>(oss, "\n"));
        return oss.str();
    }
};

namespace dave {

template <typename T>
T sort(T c) {
    std::sort(c.begin(), c.end());
    return c;
}

auto sort(auto &c, auto comp = std::less()) {
    std::sort(c.begin(), c.end(), comp);
    return c;
}
};
