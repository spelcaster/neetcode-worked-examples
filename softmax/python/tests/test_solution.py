import numpy as np
import pytest
from solution import Solution


@pytest.mark.parametrize(
    "z, expected",
    [
        # notes/problem.md Example 1
        ([1, 2, 3], [0.0900, 0.2447, 0.6652]),
        # notes/problem.md Example 2
        ([1, 1, 1], [0.3333, 0.3333, 0.3333]),
        # notes/worked-example.md section 6: all scores equal (non-zero)
        ([5.0, 5.0, 5.0], [0.3333, 0.3333, 0.3333]),
        # notes/worked-example.md section 6: single element
        ([7.0], [1.0]),
        # notes/worked-example.md section 6: negative scores
        ([-5.0, -1.0, 0.0], [0.0049, 0.2677, 0.7274]),
        # notes/worked-example.md section 3: large scores that would
        # overflow without max-shifting; identical to [1, 2, 3] shifted
        # by a constant 999.
        ([1000.0, 1001.0, 1002.0], [0.0900, 0.2447, 0.6652]),
    ],
)
def test_softmax(z, expected):
    result = Solution().softmax(np.array(z, dtype=np.float64))
    np.testing.assert_allclose(result, expected, atol=1e-4)


def test_softmax_sums_to_one():
    z = np.array([1.0, 2.0, 3.0], dtype=np.float64)
    result = Solution().softmax(z)
    assert np.sum(result) == pytest.approx(1.0, abs=1e-3)
