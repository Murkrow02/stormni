#!/bin/bash
set -e
clang++ -std=c++17 -O2 main.cpp -o murmur $(pkg-config --cflags --libs raylib)
./murmur
