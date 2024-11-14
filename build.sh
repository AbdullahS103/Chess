#! /bin/bash
cmake -S . -B build && cmake --build build
cp -f build/chess chess