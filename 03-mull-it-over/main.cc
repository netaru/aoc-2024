#include <iostream>
#include <iterator>
#include <numeric>
#include <regex>
#include <string>

int accumulator(int acc, std::smatch match) { return (std::stoi(match[1]) * std::stoi(match[2])) + acc; }
int solve(std::string s) {
    std::regex r("mul\\((\\d{1,3}),(\\d{1,3})\\)");
    return std::accumulate(std::sregex_iterator(s.begin(), s.end(), r), {}, 0, accumulator);
}

std::string remove_donts(std::string s) {
    while (true) {
        size_t from = s.find("don't()"), to = s.find("do()", from);
        if (from == std::string::npos) break;
        s.erase(from, to - from);
    }
    return s;
}

int main(int argc, char *argv[]) {
    std::string s{ std::istream_iterator<char>{ std::cin }, {} };
    std::cout << "Part1: " << solve(s) << "\n";
    std::cout << "Part2: " << solve(remove_donts(s)) << "\n";
    return 0;
}
