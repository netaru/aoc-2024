#pragma once

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstddef>
#include <functional>
#include <iterator>
#include <print>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

template <typename T = int>
std::vector<T> ints(std::string_view sv) {
    std::vector<T> result;

    size_t ind = 0;
    while (true) {
        auto iter = std::find_if(sv.begin() + ind, sv.end(), isdigit);
        auto end  = std::find_if_not(iter, sv.end(), isdigit);
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
    while (true) {
        size_t           where = sv.find(delimiter);
        std::string_view part  = trim({ sv.substr(0, where) });
        if (part.size()) {
            if constexpr (std::is_same_v<T, std::string>) {
                result.emplace_back(part);
            } else {
                if (T value; std::from_chars(part.data(), part.data() + part.size(), value).ec == std::errc{}) {
                    result.push_back(value);
                }
            }
        }
        if (where == std::string_view::npos) break;
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
