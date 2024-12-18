#pragma once

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <string>
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
