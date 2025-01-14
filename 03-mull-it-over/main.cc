#include <iostream>
#include <iterator>
#include <numeric>
#include <regex>
#include <string>

using namespace std;

int add(smatch m) { return (stoi(m[1]) * stoi(m[2])); }
int solve(string s) {
    regex r("mul\\((\\d{1,3}),(\\d{1,3})\\)");
    return transform_reduce(sregex_iterator(s.begin(), s.end(), r), {}, 0, plus(), add);
}

string remove_donts(string s, string ns = "") {
    ns.reserve(s.size());
    regex disabled("don't\\(\\).*?(?:do\\(\\))");
    regex_replace(back_inserter(ns), s.begin(), s.end(), disabled, "");
    return ns;
}

int main(int argc, char *argv[]) {
    string s{ istream_iterator<char>{ cin }, {} };
    cout << "Part1: " << solve(s) << "\n";
    cout << "Part2: " << solve(remove_donts(s)) << "\n";
    return 0;
}
