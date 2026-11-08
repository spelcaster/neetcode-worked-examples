# Worked example: ../notes/worked-example.md

import numpy as np
from numpy.typing import NDArray


class Solution:
    def forward(self, x: NDArray[np.float64], w: NDArray[np.float64], b: float, activation: str) -> float:
        # x: 1D input array
        # w: 1D weight array (same length as x)
        # b: scalar bias
        # activation: "sigmoid" or "relu"
        #
        # Pre-activation: z = dot(x, w) + b
        # Sigmoid: σ(z) = 1 / (1 + exp(-z))
        # ReLU: max(0, z)
        # return round(your_answer, 5)
        pass

    def sigmoid(self, x: float) -> float:
        # This function calculates the sigmoid value of x.
        # The sigmoid value is always between 0 and 1.
        # Use this function to convert a value to a probability.
        return 1 / (1 + np.exp(-x))

    def relu(self, x: float) -> float:
        # This function calculates the ReLU value of x.
        # The ReLU value is equal to x when x is positive.
        # The ReLU value is equal to 0 when x is not positive.
        return max(0.0, x)


