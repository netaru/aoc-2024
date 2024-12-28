#pragma once

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <print>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

constexpr bool is_negative(size_t at, std::string sv) { return at < sv.size() ? sv[at] == '-' : false; }

template <typename T = int>
std::vector<T> ints(std::string sv) {
    std::vector<T> result;

    size_t ind = 0;
    while (ind < sv.size()) {
        auto iter = std::find_if(sv.begin() + ind, sv.end(), isdigit);
        auto end  = std::find_if_not(iter, sv.end(), isdigit);
        if (iter == sv.end()) break;
        std::string s;
        if (iter != sv.begin() and *(iter - 1) == '-') { s += '-'; }
        std::copy(iter, end, std::back_inserter(s));
        if constexpr (std::is_same_v<T, int>) { result.emplace_back(std::stoi(s)); }
        if constexpr (std::is_same_v<T, unsigned>) { result.emplace_back(std::stoul(s)); }
        if constexpr (std::is_same_v<T, int64_t>) { result.emplace_back(std::stoll(s)); }
        if constexpr (std::is_same_v<T, uint64_t>) { result.emplace_back(std::stoull(s)); }
        ind = end - sv.begin();
    }
    return result;
}

inline std::string_view ltrim(std::string_view sv) {
    auto lhs = std::find_if(sv.begin(), sv.end(), [](char c) { return !std::isspace(c); });
    return sv.substr(lhs - sv.begin());
}

inline std::string_view rtrim(std::string_view sv) {
    auto rhs = std::find_if(sv.rbegin(), sv.rend(), [](char c) { return !std::isspace(c); });
    return sv.substr(0, sv.size() - (rhs - sv.rbegin()));
}

inline std::string_view trim(std::string_view sv) { return rtrim(ltrim(sv)); }

template <typename T = std::string>
std::vector<T> split(std::string_view sv, std::string_view delimiter = " ") {
    std::vector<T> result;
    while (true) {
        size_t where = sv.find(delimiter);
        if (where == std::string::npos) {
            sv = trim(sv);
            if (!sv.size()) break;
            if constexpr (std::is_same_v<T, std::string>) {
                result.emplace_back(sv);
            } else {
                if (T value; std::from_chars(sv.data(), sv.data() + sv.size(), value).ec == std::errc{}) {
                    result.push_back(value);
                }
            }
            break;
        }
        std::string_view part = trim({ sv.substr(0, where) });
        if (part.size()) {
            if constexpr (std::is_same_v<T, std::string>) {
                result.emplace_back(part);
            } else {
                if (T value; std::from_chars(part.data(), part.data() + part.size(), value).ec == std::errc{}) {
                    result.push_back(value);
                }
            }
        }
        sv = sv.substr(where + delimiter.size());
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
