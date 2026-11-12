import torch
import pytest

from solution import Solution


@pytest.mark.parametrize(
    "to_reshape, expected",
    [
        ([[1.0, 2.0, 3.0, 4.0, 5.0, 6.0]], [[1.0, 2.0], [3.0, 4.0], [5.0, 6.0]]),
    ],
)
def test_reshape(to_reshape, expected):
    solution = Solution()
    result = solution.reshape(torch.tensor(to_reshape))
    assert torch.allclose(result, torch.tensor(expected), atol=1e-4)


@pytest.mark.parametrize(
    "to_avg, expected",
    [
        ([[1.0, 2.0], [3.0, 4.0]], [2.0, 3.0]),
        # edge case: single row -> mean over dim 0 is that row unchanged
        ([[1.0, 2.0]], [1.0, 2.0]),
    ],
)
def test_average(to_avg, expected):
    solution = Solution()
    result = solution.average(torch.tensor(to_avg))
    assert torch.allclose(result, torch.tensor(expected), atol=1e-4)


@pytest.mark.parametrize(
    "cat_one, cat_two, expected",
    [
        ([[1.0, 2.0]], [[3.0, 4.0]], [[1.0, 2.0, 3.0, 4.0]]),
        # edge case: zero-width inputs still concatenate on matching row count
        ([[]], [[]], [[]]),
    ],
)
def test_concatenate(cat_one, cat_two, expected):
    solution = Solution()
    result = solution.concatenate(torch.tensor(cat_one), torch.tensor(cat_two))
    assert torch.allclose(result, torch.tensor(expected), atol=1e-4)
    assert result.shape == torch.tensor(expected).shape


@pytest.mark.parametrize(
    "prediction, target, expected",
    [
        ([1.0, 2.0, 3.0], [1.0, 2.0, 4.0], 0.3333),
        # edge case: identical prediction and target -> zero loss
        ([1.0, 2.0, 3.0], [1.0, 2.0, 3.0], 0.0),
    ],
)
def test_get_loss(prediction, target, expected):
    solution = Solution()
    result = solution.get_loss(torch.tensor(prediction), torch.tensor(target))
    assert result.shape == ()
    assert torch.allclose(result, torch.tensor(expected), atol=1e-4)
