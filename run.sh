#!/usr/bin/env bash

for i in [0-2]*;
do
    echo "Running $i..."
    "./build/$i" < "$i/input.txt"
done

