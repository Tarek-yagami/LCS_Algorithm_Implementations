#!/bin/bash

# Compile and run the sequential program
echo "Compiling Sequential Code..."
gcc -o FinalS FinalS.c
echo "Running Sequential Code..."
# Replace 'file1.txt' and 'file2.txt' with your actual test file names
./FinalS <<EOF
X_20000.txt
Y_20000.txt
EOF

# Compile and run the parallel program
echo "Compiling Parallel Code..."
gcc -pthread -o Final Final.c
echo "Running Parallel Code..."

# Loop through desired thread counts
for THREADS in 2 4 8 16
do
    echo "Running Parallel Code with $THREADS threads..."
    ./Final <<EOF
    $THREADS
    X_20000.txt
    Y_20000.txt
EOF
done