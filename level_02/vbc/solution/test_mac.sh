#!/bin/bash

# Compile the program with debugging enabled
gcc -Wall -Wextra -Werror -g -o vbc_parser vbc.c

# Check if compilation succeeded
if [ $? -ne 0 ]; then
    echo "❌ Compilation failed!"
    exit 1
fi

# Define test cases (expression | expected_output)
declare -a tests=(
    "3+2|5"
    "3*2|6"
    "(3+2)|5"
    "3+2*4|11"
    "(3+2)*4|20"
    "((2+1)*3)+4*(2+(3+2))|37"
    "9+1*2+3*4|23"
    "8*(1+2)+6*3|42"
    "1+2+3+4+5+6+7+8+9|45"
    "(1+2)*(3+4)|21"
    "0+0|0"
    "0*9|0"
    "9*9|81"
    "(((((1+1)*2)+3)*4)+5)|33"
    "(1+(2*(3+(4*(5+6)))))|95"
    "(((((((1+1)*2)+3)*4)+5)+6)*7)|273"
    "(1+2)*((3+4)*(5+6))|231"
    "((((((((((1+2)+3)+4)+5)+6)+7)+8)+9)+0))|45"
    "9*0+1|1"

    # Error cases
    "3++2|Unexpected token '+'"
    "3**2|Unexpected token '*'"
    "3+*2|Unexpected token '*'"
    "(*2+3)|Unexpected token '*'"
    "4+4)|Unexpected token ')'"
    "3+()|Unexpected token ')'"
    "3+(2*4|Unexpected end of file"
    ")|Unexpected token ')'"
    "(|Unexpected end of file"
    "((3+2)*4|Unexpected end of file"
    "3+2*)|Unexpected token ')'"
    "3+2*(4|Unexpected end of file"
    "abc+2|Unexpected token 'a'"
    "2+@3|Unexpected token '@'"
    "2+@3|Unexpected token '@'"
    "((((((1+2)*3)*4)*5)*6|Unexpected end of file"
)

# Run tests
echo "Running tests with macOS memory leak detection..."
total=${#tests[@]}
pass=0
mem_leak=0

for test in "${tests[@]}"; do
    input="${test%%|*}"
    expected="${test##*|}"

    # Run the program in the background and get its PID
    ./vbc_parser "$input" > output.txt 2>&1 &
    pid=$!

    # Wait for the process to finish
    wait $pid
    result=$(cat output.txt)

    # Compare actual and expected output
    if [ "$result" == "$expected" ]; then
        echo "✅ PASS: '$input' -> $result"
        ((pass++))
    else
        echo "❌ FAIL: '$input'"
        echo "   Expected: $expected"
        echo "   Got:      $result"
    fi

    # Check for memory leaks using macOS `leaks`
    leaks_output=$(leaks $pid 2>&1)
    if echo "$leaks_output" | grep -q "leaked"; then
        echo "   ⚠️ Memory leak detected in test: '$input'"
        ((mem_leak++))
    fi
done

# Summary
echo "=============================="
echo "Tests Passed: $pass / $total"
echo "Memory Leaks Detected: $mem_leak"
echo "=============================="

# Exit with error code if tests fail or memory leaks are found
if [ $pass -ne $total ] || [ $mem_leak -ne 0 ]; then
    exit 1
else
    exit 0
fi
