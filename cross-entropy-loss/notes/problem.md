# Cross-Entropy Loss

https://neetcode.io/problems/cross-entropy-loss

## Description

*Prerequisite: Solve [Softmax](https://neetcode.io/problems/softmax) first.*

Softmax gives you probabilities, the model's confidence for each class.
But how do you tell the model how wrong it was? That's what
cross-entropy loss does. It compares the predicted distribution to the
actual answer and produces a single number: high when the model is
confidently wrong, low when it's right. This is the loss function
behind almost every classifier and every token prediction in GPT.

The lower the loss, the better. A perfect prediction gives a loss of 0.

**Binary Cross-Entropy** (for two classes):

```
L = -(1/n) * sum_{i=1..n} [ y_i * ln(p_i) + (1 - y_i) * ln(1 - p_i) ]
```

Where `y_i` is the true label (0 or 1) and `p_i` is the predicted
probability.

**Categorical Cross-Entropy** (for multiple classes):

```
L = -(1/n) * sum_{i=1..n} sum_{c=1..C} y_{i,c} * ln(p_{i,c})
```

Where `y_{i,c}` is 1 if sample `i` belongs to class `c` (one-hot
encoded), and `p_{i,c}` is the predicted probability.

**Inputs:**
- For binary: `y_true` (true labels, 0 or 1) and `y_pred` (predicted
  probabilities)
- For categorical: `y_true` (one-hot encoded) and `y_pred` (predicted
  probabilities per class)

Round your result to 4 decimal places.

Implement both as methods on `Solution`:

```python
class Solution:
    def binary_cross_entropy(self, y_true: NDArray[np.float64], y_pred: NDArray[np.float64]) -> float:
        # y_true: true labels (0 or 1)
        # y_pred: predicted probabilities
        # Hint: clip y_pred to [1e-7, 1 - 1e-7] to avoid log(0)
        # return round(your_answer, 4)
        ...

    def categorical_cross_entropy(self, y_true: NDArray[np.float64], y_pred: NDArray[np.float64]) -> float:
        # y_true: one-hot encoded true labels (shape: n_samples x n_classes)
        # y_pred: predicted probabilities (shape: n_samples x n_classes)
        # Hint: clip y_pred to [1e-7, 1 - 1e-7] to avoid log(0)
        # return round(your_answer, 4)
        ...
```

## Examples

**Example 1 (Binary):**

```
Input: y_true = [1, 0, 1]
       y_pred = [0.9, 0.1, 0.8]
Output: 0.1446
```

For each sample, we use the true label to pick which term applies.
Sample 1 (`y = 1`): we take `ln(0.9)`. Sample 2 (`y = 0`): we take
`ln(1 - 0.1) = ln(0.9)`. Sample 3 (`y = 1`): we take `ln(0.8)`. Average
loss: `-(1/3)[ln(0.9) + ln(0.9) + ln(0.8)] = 0.1446`. The model is
confident and correct, so loss is low.

**Example 2 (Categorical):**

```
Input: y_true = [[1,0,0], [0,1,0]]
       y_pred = [[0.9,0.05,0.05], [0.1,0.8,0.1]]
Output: 0.1643
```

## Constraints

- Round the result to 4 decimal places.
- Clip `y_pred` to `[1e-7, 1 - 1e-7]` before taking `ln` to avoid
  `log(0)`.
- Binary case: `y_true` is a 1D array of 0/1 labels, `y_pred` a 1D
  array of probabilities, same length.
- Categorical case: `y_true` is one-hot encoded with shape
  `(n_samples, n_classes)`, `y_pred` has matching shape.
