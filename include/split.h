#pragma once

#include <cstdint>
#include <string>
#include <vector>

template <typename T = std::string>
std::vector<T> split(std::string s, char delim = ' ') {
    std::vector<T> result;
    while (true) {
        size_t where = s.find(delim);
        if (where == std::string::npos) {
            if (!s.size()) break;
            if constexpr (std::is_same_v<T, int>) {
                result.push_back(std::stoi(s));
            } else if constexpr (std::is_same_v<T, std::uint64_t>) {
                result.push_back(std::stoull(s));
            } else {
                result.push_back(s);
            }
            break;
        }
        std::string part{ s.substr(0, where) };
        if (part.size()) {
            if constexpr (std::is_same_v<T, int> or std::is_same_v<T, std::uint64_t>) {
                result.push_back(std::stoi(part));
            } else if constexpr (std::is_same_v<T, std::uint64_t>) {
                result.push_back(std::stoull(s));
            } else {
                result.push_back(part);
            }
        }
        s = s.substr(where + 1);
    }
    return result;
}
