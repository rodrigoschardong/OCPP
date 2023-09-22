#!/bin/bash

# Check if the "build" directory exists
if [ ! -d "build" ]; then
    # If it doesn't exist, create it
    mkdir build
fi

# Compile your code
echo "Compiling your code"
gcc -o build/client_bin charge_point.c -lwebsockets
echo "Code compiled"
# Run the program
echo "Running the program"
./build/client_bin
