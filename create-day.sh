#!/usr/bin/env bash

if [ -d "$1" ]; then
    echo "$1" Already exists..
    exit 1
fi

mkdir "$1"

touch "$1/test.txt"
touch "$1/input.txt"
touch "$1/instructions.txt"

cat > "$1/main.cc"  <<EOF
#include <iostream>

int main(int argc, char *argv[]) {
    std::cout << "This is the result\n";
    return 0;
}
EOF
