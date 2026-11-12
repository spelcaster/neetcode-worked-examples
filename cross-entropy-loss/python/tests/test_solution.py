import numpy as np
import pytest

from solution import Solution


def test_binary_cross_entropy_example():
    solution = Solution()
    result = solution.binary_cross_entropy(np.array([1, 0, 1]), np.array([0.9, 0.1, 0.8]))
    assert result == pytest.approx(0.1446, abs=1e-4)


def test_binary_cross_entropy_perfect_prediction():
    solution = Solution()
    result = solution.binary_cross_entropy(np.array([1, 0]), np.array([1.0, 0.0]))
    assert result == pytest.approx(0.0, abs=1e-4)


def test_binary_cross_entropy_zero_probability_is_clipped_not_crashing():
    solution = Solution()
    result = solution.binary_cross_entropy(np.array([1]), np.array([0.0]))
    assert result == pytest.approx(-np.log(1e-7), abs=1e-2)


def test_binary_cross_entropy_single_sample():
    solution = Solution()
    result = solution.binary_cross_entropy(np.array([1]), np.array([0.8]))
    assert result == pytest.approx(-np.log(0.8), abs=1e-4)


def test_categorical_cross_entropy_example():
    solution = Solution()
    y_true = np.array([[1, 0, 0], [0, 1, 0]])
    y_pred = np.array([[0.9, 0.05, 0.05], [0.1, 0.8, 0.1]])
    result = solution.categorical_cross_entropy(y_true, y_pred)
    assert result == pytest.approx(0.1643, abs=1e-4)


def test_categorical_cross_entropy_perfect_prediction():
    solution = Solution()
    y_true = np.array([[1, 0], [0, 1]])
    y_pred = np.array([[1.0, 0.0], [0.0, 1.0]])
    result = solution.categorical_cross_entropy(y_true, y_pred)
    assert result == pytest.approx(0.0, abs=1e-4)


def test_categorical_cross_entropy_zero_probability_is_clipped_not_crashing():
    solution = Solution()
    y_true = np.array([[1, 0]])
    y_pred = np.array([[0.0, 1.0]])
    result = solution.categorical_cross_entropy(y_true, y_pred)
    assert result == pytest.approx(-np.log(1e-7), abs=1e-2)


def test_categorical_cross_entropy_single_sample():
    solution = Solution()
    y_true = np.array([[0, 1, 0]])
    y_pred = np.array([[0.1, 0.8, 0.1]])
    result = solution.categorical_cross_entropy(y_true, y_pred)
    assert result == pytest.approx(-np.log(0.8), abs=1e-4)


def test_binary_and_categorical_agree_on_equivalent_input():
    solution = Solution()
    binary_result = solution.binary_cross_entropy(np.array([1, 0]), np.array([0.9, 0.2]))
    y_true = np.array([[0, 1], [1, 0]])
    y_pred = np.array([[0.1, 0.9], [0.8, 0.2]])
    categorical_result = solution.categorical_cross_entropy(y_true, y_pred)
    assert binary_result == pytest.approx(categorical_result, abs=1e-4)
