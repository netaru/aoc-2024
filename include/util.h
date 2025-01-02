#pragma once

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstddef>
#include <functional>
#include <istream>
#include <iterator>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>
#include <vector>

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
