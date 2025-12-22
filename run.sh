#!/bin/bash

# Check if the build directory exists
if [ ! -d "build" ]; then
    mkdir build
    cd build || { echo "Failed to change directory to build"; exit 1; }
    cmake -B . -S ../
else
    cd build || { echo "Failed to change directory to build"; exit 1; }
    cmake -B . -S ../
fi

# Run make
make && ./out