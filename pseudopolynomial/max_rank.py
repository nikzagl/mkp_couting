import itertools
import numpy as np

def max_abs_rank_minor(matrix):
    matrix = np.array(matrix, dtype=float)
    rows, cols = matrix.shape
    max_k = min(rows, cols)

    best_value = 0
    best_minor = None
    best_submatrix = None
    best_order = 0

    # сначала ищем максимальный порядок (ранг)
    for k in range(max_k, 0, -1):
        found = False
        candidates = []

        for row_indices in itertools.combinations(range(rows), k):
            for col_indices in itertools.combinations(range(cols), k):
                submatrix = matrix[np.ix_(row_indices, col_indices)]
                det = np.linalg.det(submatrix)

                if abs(det) > 1e-9:
                    found = True
                    candidates.append((det, submatrix))

        if found:
            # среди всех миноров порядка k выбираем максимальный по модулю
            best_det, best_sub = max(candidates, key=lambda x: abs(x[0]))
            median_det = np.median([abs(c[0]) for c in candidates])
            return {
                "order": k,
                "value": best_det,
                "abs_value": abs(best_det),
                "submatrix": best_sub,
                "median_abs_value": median_det
            }

    return None


# Пример
f = open(r"pseudopolynomial\input_data\test_2\knap_n_20.txt", "r")
lines = f.readlines()
f.close()
n,m = map(int, lines[0].split())
A = []
for i in range(1, m+1):
    values = list(map(int, lines[i].split()))
    A.append(values)

result = max_abs_rank_minor(A)

if result:
    print("Порядок:", result["order"])
    print("Определитель:", result["value"])
    print("Модуль:", result["abs_value"])
    print("Подматрица:\n", result["submatrix"])
    print("Медиана значений по модулю для всех миноров этого порядка:", result["median_abs_value"])
else:
    print("Матрица нулевая")
