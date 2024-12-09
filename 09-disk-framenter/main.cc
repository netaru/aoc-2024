#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <istream>
#include <iterator>
#include <string>
#include <vector>

struct chunk {
    int  id;
    bool is_file() const { return id != -1; }
};

using memory_t = std::vector<chunk>;

memory_t create_chunks(size_t sz, int id) {
    chunk    c{ .id = id };
    memory_t result(sz, c);
    return result;
}

memory_t parse(std::istream& is) {
    std::string s;
    memory_t    result;
    int         id = 0;
    std::getline(is, s);
    bool is_file = true;
    for (const char c : s) {
        if (is_file) {
            memory_t chunks = create_chunks(c - '0', id++);
            std::copy(chunks.begin(), chunks.end(), std::back_inserter(result));
        } else {
            memory_t chunks = create_chunks(c - '0', -1);
            std::copy(chunks.begin(), chunks.end(), std::back_inserter(result));
        }
        is_file = !is_file;
    }
    return result;
}

void print(memory_t m) {
    for (auto block : m) {
        if (block.is_file())
            std::cout << block.id;
        else
            std::cout << '.';
    }
    std::cout << "\n";
}

void swap(auto first, auto second) {
    chunk tmp = *first;
    *first    = *second;
    *second   = tmp;
}

memory_t::reverse_iterator find_next(memory_t& in, memory_t::reverse_iterator iter) {
    return std::find_if(iter, in.rend(), [](const chunk& ch) { return ch.is_file(); });
}

bool can_defrag(memory_t& in, memory_t::reverse_iterator iter) {
    return std::find_if(iter, in.rend(), [](const chunk& ch) { return !ch.is_file(); }) != in.rend();
}

memory_t defrag(memory_t in) {
    for (memory_t::reverse_iterator iter = find_next(in, in.rbegin()); can_defrag(in, iter);
         iter                            = find_next(in, iter)) {
        swap(std::find_if(in.begin(), in.end(), [](const chunk& ch) { return !ch.is_file(); }), iter);
    }
    return in;
}

size_t count(memory_t in) {
    size_t sum = 0;
    for (size_t u = 0; in[u].is_file(); ++u) {
        sum += (u * in[u].id);
    }
    return sum;
}

int main(int argc, char* argv[]) {
    auto memory = parse(std::cin);
    // print(memory);
    memory = defrag(memory);
    // print(memory);
    std::cout << "Part1: " << count(memory) << "\n";
    return 0;
}
