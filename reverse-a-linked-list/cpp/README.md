# Reverse a Linked List

https://neetcode.io/problems/reverse-a-linked-list

Problem statement: `../notes/problem.md`
Worked example / step-by-step walkthrough: `../notes/worked-example.md`

## Layout

```
include/solution.hpp   ListNode definition + function signature
src/solution.cpp       implementation (TODO)
src/main.cpp           interactive runner
tests/test_solution.cpp  assert-based test cases
```

## Build & run

```sh
cmake -S . -B build
cmake --build build

./build/main          # interactive
./build/tests         # run test cases
ctest --test-dir build  # same, via ctest
```
