# Softmax

https://neetcode.io/problems/softmax

*Prerequisite: Solve [Sigmoid & ReLU](https://neetcode.io/problems/sigmoid-and-relu) first.*

## Description

Softmax converts a vector of raw scores (called **logits**) into a
probability distribution. The output values are all positive and sum to
1, making them interpretable as probabilities.

This is how models like GPT decide which token comes next. The final
layer outputs a score for every possible token, and softmax turns those
scores into probabilities.

The formula for the `i`-th element is:

```
softmax(z)_i = e^(z_i) / sum_j(e^(z_j))
```

**Important:** for numerical stability, subtract the maximum value from
all elements before computing:

```
softmax(z)_i = e^(z_i - max(z)) / sum_j(e^(z_j - max(z)))
```

This prevents overflow when the input values are large.

**Input:**
- `z` - a 1D NumPy array of logits

Round your result to 4 decimal places.

**Function signature (Python starter):**

```python
import numpy as np
from numpy.typing import NDArray

class Solution:
    def softmax(self, z: NDArray[np.float64]) -> NDArray[np.float64]:
        # z is a 1D NumPy array of logits
        # Hint: subtract max(z) for numerical stability before computing exp
        # return np.round(your_answer, 4)
        pass
```

## Examples

**Example 1:**
```
Input: z = [1, 2, 3]
Output: [0.09, 0.2447, 0.6652]
```
Explanation: `e^1 = 2.718`, `e^2 = 7.389`, `e^3 = 20.086`. Sum = `30.193`.
So softmax = `[2.718/30.193, 7.389/30.193, 20.086/30.193]`.

**Example 2:**
```
Input: z = [1, 1, 1]
Output: [0.3333, 0.3333, 0.3333]
```
Explanation: Equal inputs produce a uniform distribution.

## Constraints

- `z` is a 1D NumPy array of `float64` logits.
- Difficulty: Easy. Topic: Math Foundations.
- Result must be rounded to 4 decimal places.
