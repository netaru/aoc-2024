#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <istream>
#include <iterator>
#include <string>
#include <vector>

using memory_t = std::vector<int>;

memory_t parse(std::istream& is) {
    memory_t memory;

    std::string s;
    std::getline(is, s);
    for (size_t u = 0; u < s.size(); ++u) {
        memory_t chunk(s[u] - '0', (u + 1) % 2 ? u / 2 : -1);
        std::copy(chunk.begin(), chunk.end(), std::back_inserter(memory));
    }
    return memory;
}

bool can_possibly_defrag(memory_t& in, memory_t::reverse_iterator iter) {
    return std::find_if(iter, in.rend(), [](const int& i) { return i == -1; }) != in.rend();
}

size_t chunk_size(auto iter) {
    size_t sz    = 0;
    int    value = (*iter);
    while ((*iter) == value) {
        iter++;
        sz++;
    }
    return sz;
}

memory_t::reverse_iterator first_chunk(memory_t& in, memory_t::reverse_iterator iter) {
    return std::find_if(iter, in.rend(), [](const int& i) { return i != -1; });
}

memory_t::reverse_iterator find_next_chunk(memory_t& in, memory_t::reverse_iterator iter) {
    return std::find_if(iter, in.rend(), [&](const int& i) { return i != -1 && i != *iter; });
}

memory_t::iterator find_available(memory_t::iterator from, memory_t::iterator stop, size_t sz) {
    while (from < stop) {
        from             = std::find_if(from, stop, [](const int& i) { return i == -1; });
        size_t available = chunk_size(from);
        if (available >= sz) return from;
        from += available;
    }
    return from;
}

void swap(auto iter1, auto iter2, size_t sz) {
    for (size_t u = 0; u < sz; u++) { std::swap(*iter1++, *iter2++); }
}

template <int part = 1>
memory_t defrag(memory_t in) {
    for (auto iter = first_chunk(in, in.rbegin()); can_possibly_defrag(in, iter); iter = find_next_chunk(in, iter)) {
        if constexpr (part == 1) {
            std::swap(*std::find_if(in.begin(), in.end(), [](const int& i) { return i == -1; }), *iter);
            continue;
        }
        size_t sz   = chunk_size(iter);
        auto   stop = in.begin() + (std::distance(iter, in.rend()) - 1), where = find_available(in.begin(), stop, sz);
        if (where < stop) { swap(iter, where, sz); }
    }
    return in;
}

size_t count(memory_t in) {
    size_t sum = 0;
    for (size_t u = 0; u < in.size(); ++u) { sum += (u * std::max(0, in[u])); }
    return sum;
}

int main(int argc, char* argv[]) {
    memory_t memory = parse(std::cin);
    std::cout << "Part1: " << count(defrag(memory)) << "\n";
    std::cout << "Part2: " << count(defrag<2>(memory)) << "\n";
    return 0;
}
