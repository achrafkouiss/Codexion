#!/usr/bin/env bash

BIN="${1:-./a.out}"

tests=(
"1 100 200 200 200 1 0 fifo"
"1 800 200 200 200 5 0 fifo"
"1 1000 60 60 60 100 0 fifo"

"2 800 200 200 200 3 0 fifo"
"2 800 200 200 200 3 50 fifo"
"2 300 200 200 200 3 0 fifo"

"3 800 200 200 200 5 0 fifo"
"3 400 200 200 200 5 0 fifo"
"3 250 200 200 200 5 0 fifo"

"50 1000 200 200 200 3 0 fifo"
"100 1000 200 200 200 3 0 fifo"
"200 1000 200 200 200 3 0 fifo"

"5 150 200 200 200 5 0 fifo"
"20 150 200 200 200 5 0 fifo"
"50 150 200 200 200 5 0 fifo"

"5 100 200 10 10 5 0 fifo"

"5 5000 100 100 100 10 500 fifo"
"5 5000 100 100 100 10 1000 fifo"
"20 5000 100 100 100 10 1000 fifo"

"5 5000 100 100 100 2 0 fifo"
"50 5000 50 50 50 2 0 fifo"
"100 5000 10 10 10 1 0 fifo"

"300 5000 10 10 10 1 0 fifo"
"500 5000 10 10 10 1 0 fifo"

"100 5000 50 50 50 1 0 fifo"

"200 800 150 150 150 20 100 fifo"
)

pass=0
fail=0
i=1

for t in "${tests[@]}"; do
    echo "=================================================="
    echo "TEST $i: $BIN $t"
    echo "=================================================="

    timeout 30s $BIN $t > test_output.log 2>&1
    status=$?

    if [ $status -eq 0 ]; then
        echo "[PASS] Exit code 0"
        pass=$((pass+1))
    elif [ $status -eq 124 ]; then
        echo "[FAIL] Timed out (possible deadlock)"
        fail=$((fail+1))
    else
        echo "[FAIL] Exit code $status"
        fail=$((fail+1))
    fi

    if grep -q "ThreadSanitizer" test_output.log; then
        echo "[FAIL] TSAN warning detected"
        fail=$((fail+1))
    fi

    i=$((i+1))
done

echo
echo "============== SUMMARY =============="
echo "Passed: $pass"
echo "Failed: $fail"
echo "====================================="
