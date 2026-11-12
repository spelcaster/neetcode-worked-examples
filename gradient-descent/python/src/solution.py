class Solution:
    def get_minimizer(self, iterations: int, learning_rate: float, init: int) -> float:
        # Objective function: f(x) = x^2
        # Derivative:         f'(x) = 2x
        # Update rule:        x = x - learning_rate * f'(x)
        # Round final answer to 5 decimal places

        if iterations == 0:
            return init

        minVal = float('inf')
        x = init

        for n in range(iterations):
            x = x - learning_rate * (2*x)

            if (abs(x) < abs(minVal)):
                minVal = x

        return round(minVal, 5)
