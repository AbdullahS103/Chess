#! /bin/bash
cmake -S . -B build && cmake --build build && GTEST_COLOR=1 ctest --test-dir build --output-on-failure
cp -f build/chess chess