#!/bin/bash
# Compila ed esegue lo step.
# -std=c++17  : versione di C++
# -O2         : ottimizzazione, rende fluida l'animazione
# pkg-config  : aggiunge automaticamente i flag per linkare raylib
set -e
clang++ -std=c++17 -O2 main.cpp -o murmur $(pkg-config --cflags --libs raylib)
./murmur
