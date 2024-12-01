#pragma once

#include <string>
#include <vector>

template <typename T = std::string>
std::vector<T> split(std::string s, char delim = ' ') {
    std::vector<T> result;
    while (true) {
        size_t where = s.find(delim);
        if (where == std::string::npos) {
            if constexpr (std::is_same_v<T, int>) {
                if (s.size()) result.push_back(std::stoi(s));
            } else {
                if (s.size()) result.push_back(s);
            }
            break;
        }
        std::string part{ s.substr(0, where) };
        if constexpr (std::is_same_v<T, int>) {
            if (part.size()) result.push_back(std::stoi(part));
        } else {
            if (part.size()) result.push_back(part);
        }
        s = s.substr(where + 1);
    }
    return result;
}
