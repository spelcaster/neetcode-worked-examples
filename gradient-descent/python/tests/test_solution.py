import numpy as np
import pytest

from solution import Solution


@pytest.mark.parametrize(
    "iterations, learning_rate, init, expected",
    [
        (0, 0.01, 5, 5),
        (10, 0.01, 5, 4.08536),
        (10, 0.05, -3, -1.04604),
    ],
)
def test_forward(iterations, learning_rate, init, expected):
    solution = Solution()
    result = solution.get_minimizer(iterations=iterations, learning_rate=learning_rate, init=init)
    assert result == pytest.approx(expected)
