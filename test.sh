#!/bin/bash
binary="./conv"

gcc -fopenmp main.c -o conv

for i in {1..10}; do
    input_file="./tests/${i}.in"
    expected_output_file="./tests/${i}.out"
    temp_output_file="temp_${i}.out"

    $binary < "$input_file" > "$temp_output_file"

    if diff -q "$temp_output_file" "$expected_output_file" > /dev/null; then
        echo "Test $i passed."
    else
        echo "Test $i failed."
    fi

    rm "$temp_output_file"
done

rm ./conv