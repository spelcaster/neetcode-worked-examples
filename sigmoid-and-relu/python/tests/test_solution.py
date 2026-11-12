import numpy as np
import pytest

from solution import Solution


@pytest.mark.parametrize(
    "z, expected",
    [
        ([2.0], [0.88080]),
        ([0.0], [0.5]),
        ([-50.0], [0.0]),
        ([0.37], [0.59146]),
    ],
)
def test_sigmoid(z, expected):
    solution = Solution()
    result = solution.sigmoid(np.array(z))
    assert result == pytest.approx(expected, abs=1e-5)


@pytest.mark.parametrize(
    "z, expected",
    [
        ([2.0], [2.0]),
        ([0.0], [0.0]),
        ([-50.0], [0.0]),
        ([0.37], [0.37]),
    ],
)
def test_relu(z, expected):
    solution = Solution()
    result = solution.relu(np.array(z))
    assert result == pytest.approx(expected, abs=1e-5)
