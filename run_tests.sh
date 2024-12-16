#!/bin/bash

# Define an array with different values for NUM_ELEMENTS
NUM_ELEMENTS_ARRAY=(5 25 150 500 1000 2500 5000 10000 20000 25000 30000 40000) # Add more values as needed
# Log file for storing output
LOG_FILE="output.log"

# Clear the log file at the start (optional)
> $LOG_FILE

# Compile and run the sequential program
echo "Compiling Sequential Code..." | tee -a $LOG_FILE
gcc -o FinalS FinalS.c
echo "Running Sequential Code..." | tee -a $LOG_FILE

# Loop through the array of NUM_ELEMENTS values
for NUM_ELEMENTS in "${NUM_ELEMENTS_ARRAY[@]}"
do
    echo "Running Sequential Code for NUM_ELEMENTS=$NUM_ELEMENTS..." | tee -a $LOG_FILE
    ./FinalS <<EOF | tee -a $LOG_FILE
    X_${NUM_ELEMENTS}.txt
    Y_${NUM_ELEMENTS}.txt
EOF
done

# Compile and run the parallel program
echo "Compiling Parallel Code..." | tee -a $LOG_FILE
gcc -pthread -o Final Final.c
echo "Running Parallel Code..." | tee -a $LOG_FILE

# Loop through the array of NUM_ELEMENTS values and desired thread counts
for NUM_ELEMENTS in "${NUM_ELEMENTS_ARRAY[@]}"
do
    for THREADS in 2 4 8 16
    do
        echo "Running Parallel Code for NUM_ELEMENTS=$NUM_ELEMENTS with $THREADS threads..." | tee -a $LOG_FILE
        ./Final <<EOF | tee -a $LOG_FILE
        $THREADS
        X_${NUM_ELEMENTS}.txt
        Y_${NUM_ELEMENTS}.txt
EOF
    done
done

# Compile and run the CUDA program
echo "Compiling CUDA Code..." | tee -a $LOG_FILE
nvcc -o FinalCuda prog.cu
echo "Running CUDA Code..." | tee -a $LOG_FILE

# Loop through the array of NUM_ELEMENTS values
for NUM_ELEMENTS in "${NUM_ELEMENTS_ARRAY[@]}"
do
    echo "Running CUDA Code for NUM_ELEMENTS=$NUM_ELEMENTS..." | tee -a $LOG_FILE
    ./FinalCuda <<EOF | tee -a $LOG_FILE
    X_${NUM_ELEMENTS}.txt
    Y_${NUM_ELEMENTS}.txt
EOF
done