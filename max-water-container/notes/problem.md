# Container With Most Water

https://neetcode.io/problems/max-water-container/question?list=neetcode150

## Description

You are given an integer array `heights` where `heights[i]` represents the
height of the `i`-th bar.

You may choose any two bars to form a container. Return the maximum amount
of water such a container can store.

**Note:** the problem statement names the parameter `heights`, but the
example inputs below use `height` — both refer to the same array.

**Function signature (C++):**
```cpp
int maxArea(vector<int>& heights);
```

Recommended complexity: O(n) time, O(1) space.

## Examples

**Example 1:**

```
Input: height = [1,7,2,5,4,7,3,6]
Output: 36
```

Explanation: the bars at indices 1 and 7 (heights 7 and 6) give width
`7 - 1 = 6` and height `min(7, 6) = 6`, so area `6 * 6 = 36` — the maximum
over all pairs.

**Example 2:**

```
Input: height = [2,2,2]
Output: 4
```

## Constraints

- `2 <= height.length <= 100,000`
- `0 <= height[i] <= 10,000`
