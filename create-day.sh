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
#include <print>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    println("This is the result");
    return 0;
}
EOF
