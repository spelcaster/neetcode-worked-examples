import numpy as np
import pytest

from solution import Solution


@pytest.mark.parametrize(
    "x, w, b, activation, expected",
    [
        ([1.0, 2.0], [0.5, 0.5], 0.0, "sigmoid", 0.81757),
        ([1.0, -2.0, 3.0], [0.1, 0.2, 0.3], -0.5, "relu", 0.1),
    ],
)
def test_forward(x, w, b, activation, expected):
    solution = Solution()
    result = solution.forward(np.array(x), np.array(w), b, activation)
    assert result == pytest.approx(expected, abs=1e-5)
