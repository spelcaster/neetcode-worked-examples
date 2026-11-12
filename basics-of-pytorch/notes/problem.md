# Basics of PyTorch

Problem: https://neetcode.io/problems/basics-of-pytorch

> **Note on source:** neetcode.io renders this page client-side, and a
> direct fetch returned only the page shell (title, no body). The
> description and constraints below are reconstructed from the actual
> starter code (pasted by the user) plus general knowledge of this
> problem family — not scraped verbatim from the page. The starter
> code itself (signatures, comments) is the real, confirmed source;
> example numbers below are illustrative, not copied from the live page.

## Description

Implement a `Solution` class with four small PyTorch tensor operations.
Unlike a typical plain-Python problem, inputs and outputs here are
**already `torch.Tensor` objects** (typed as `TensorType[float]`), not
Python lists — the point of the exercise is calling the right PyTorch
primitive on a tensor you're handed, not building tensors yourself.
All results should be rounded to 4 decimal places via
`torch.round(tensor, decimals=4)`.

Starter code (the real, confirmed source for all four signatures):

```python
import torch
import torch.nn
from torchtyping import TensorType

# Round all answers to 4 decimal places: torch.round(tensor, decimals=4)
class Solution:
    def reshape(self, to_reshape: TensorType[float]) -> TensorType[float]:
        # Reshape (M, N) tensor to (M*N/2, 2)
        # Use torch.reshape(tensor, new_shape)
        pass

    def average(self, to_avg: TensorType[float]) -> TensorType[float]:
        # Compute column-wise mean (average across rows)
        # Use torch.mean(tensor, dim=0)
        pass

    def concatenate(self, cat_one: TensorType[float], cat_two: TensorType[float]) -> TensorType[float]:
        # Join two tensors side-by-side along dim=1
        # Use torch.cat((a, b), dim=1)
        pass

    def get_loss(self, prediction: TensorType[float], target: TensorType[float]) -> TensorType[float]:
        # Compute Mean Squared Error between prediction and target
        # Use torch.nn.functional.mse_loss(prediction, target)
        pass
```

The four operations:

1. **`reshape(to_reshape)`** — given an `(M, N)` tensor, reshape it to
   `(M*N/2, 2)` (2 columns, rows inferred from total element count).
2. **`average(to_avg)`** — given a 2D tensor, compute the column-wise
   mean (average across rows, i.e. `dim=0`), producing a 1D tensor.
3. **`concatenate(cat_one, cat_two)`** — given two 2D tensors with the
   same number of rows, join them side-by-side along `dim=1`.
4. **`get_loss(prediction, target)`** — given two tensors of the same
   shape, compute the mean squared error between them (a scalar tensor).

## Examples

> Illustrative, not copied from the live page — numeric values follow
> directly from the formulas named in the starter comments.

**`reshape`**
```
Input:  to_reshape = tensor([[1., 2., 3., 4., 5., 6.]])   # shape (1, 6)
Output: tensor([[1., 2.], [3., 4.], [5., 6.]])             # shape (3, 2)
```

**`average`**
```
Input:  to_avg = tensor([[1., 2.], [3., 4.]])   # shape (2, 2)
Output: tensor([2., 3.])                         # shape (2,)
```

**`concatenate`**
```
Input:  cat_one = tensor([[1., 2.]]), cat_two = tensor([[3., 4.]])
Output: tensor([[1., 2., 3., 4.]])   # shape (1, 4)
```

**`get_loss`**
```
Input:  prediction = tensor([1., 2., 3.]), target = tensor([1., 2., 4.])
Output: tensor(0.3333)   # mean of (0)^2, (0)^2, (-1)^2 = 1/3, rounded to 4 decimals
```

## Constraints

- All inputs are already `torch.Tensor` objects (no list-to-tensor
  conversion needed inside the solution).
- `reshape`'s input's total element count (`M*N`) is divisible by 2.
- `average` reduces over `dim=0` only (column-wise mean).
- `concatenate`'s two inputs have the same number of rows (`dim=0`
  size); joined along `dim=1`.
- `get_loss`'s two inputs have the same shape.
- All returned tensors are rounded to 4 decimal places via
  `torch.round(tensor, decimals=4)`.
