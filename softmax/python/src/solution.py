# Worked example: ../notes/worked-example.md
import numpy as np
from numpy.typing import NDArray


class Solution:
    def softmax(self, z: NDArray[np.float64]) -> NDArray[np.float64]:
        # z is a 1D NumPy array of logits
        # Hint: subtract max(z) for numerical stability before computing exp
        # return np.round(your_answer, 4)
        maxVal = max(z)

        z -= maxVal

        k = []

        for v in z:
            k.append(np.exp(v))

        sumZ = np.sum(k)

        result = []

        for v in z:
            j = np.round(np.exp(v) / sumZ, 4)

            result.append(j)

        return np.array(result, dtype=np.float64)


