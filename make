#! /bin/bash

# Deploy keyword builds and makes the project
if [ "$1" == "build" ]; then
	echo "Starting the service..."
	cmake -S . -B build && cmake --build build
	cp -f build/chess chess

# Test is like deploy, but also runs unit tests
elif [ "$1" == "build_and_test" ]; then
	cmake -S . -B build && cmake --build build && GTEST_COLOR=1 ctest --test-dir build --output-on-failure
	cp -f build/chess chess

# Clean simply removes chess executable
elif [ "$1" == "clean" ]; then
	rm -f chess 
	rm -rf build/

else
  echo "Unknown command: $1"
fi