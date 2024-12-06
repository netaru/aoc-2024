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

std::string remove_donts(std::string s, std::string ns = "") {
    ns.reserve(s.size());
    std::regex disabled("don't\\(\\).*?(?:do\\(\\))");
    std::regex_replace(std::back_inserter(ns), s.begin(), s.end(), disabled, "");
    return ns;
}

int main(int argc, char *argv[]) {
    std::string s{ std::istream_iterator<char>{ std::cin }, {} };
    std::cout << "Part1: " << solve(s) << "\n";
    std::cout << "Part2: " << solve(remove_donts(s)) << "\n";
    return 0;
}
