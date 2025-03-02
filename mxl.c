#include "mxl.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

Matrix_Op_Result Matrix_Init(Matrix *matrix, const int r, const int c) {
	matrix->rows = r;
	matrix->cols = c;
	matrix->raw = malloc(r * c * sizeof(scalar));
	if (matrix->raw == NULL) {
		return OP_MEMORY_ERROR;
	}

	return OP_OK;
}

Matrix_Op_Result Matrix_Identity(Matrix *m, const int r, const int c) {
	Matrix_Op_Result res = Matrix_Init(m, r, c);
	ASSERT_STATUS(res);

	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			if (i == j) {
				MAT_ELEM(m, i, j) = 1;
			} else {
				MAT_ELEM(m, i, j) = 0;
			}
		}
	}

	return OP_OK;
}

Matrix_Op_Result Matrix_Zeros(Matrix *m, const int r, const int c) {
	Matrix_Op_Result res = Matrix_Init(m, r, c);
	ASSERT_STATUS(res);

	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			MAT_ELEM(m, i, j) = 0;
		}
	}

	return OP_OK;
}

void Matrix_Free(Matrix *matrix) {
	if (matrix->raw != NULL) {
		free(matrix->raw);
	}
}

Matrix_Op_Result Matrix_Euclidean_Norm(Matrix *vector, scalar *norm) {
	if (vector->cols != 1 && vector->rows != 1) {
		return OP_DIMENSION_MISMATCH;
	}

	if (vector->cols == 1) {
		for (int i = 0; i < vector->rows; i++) {
			*norm += powf(MAT_ELEM(vector, i, 1), 2.0f);
		}
		*norm = sqrtf(*norm);
	}

	if (vector->rows == 1) {
		for (int j = 0; j < vector->cols; j++) {
			*norm += powf(MAT_ELEM(vector, 1, j), 2.0f);
		}
		*norm = sqrtf(*norm);
	}

	return OP_OK;
}

Matrix_Op_Result Matrix_SetElement(Matrix *m, const int r, const int c, scalar elem) {
	if (r >= m->rows || c >= m->cols) {
		return OP_INDEX_OUT_OF_BOUND;
	}

	MAT_ELEM(m, r, c) = elem;

	return OP_OK;
}

Matrix_Op_Result Matrix_GetElement(Matrix *m, const int r, const int c, scalar *elem) {
	if (r >= m->rows || c >= m->cols) {
		return OP_INDEX_OUT_OF_BOUND;
	}

	*elem = MAT_ELEM(m, r, c);

	return OP_OK;
}

void Matrix_FPrint(FILE *stream, Matrix *m) {
	fprintf(stream, "[\n");
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			fprintf(stream, "%.2f,", MAT_ELEM(m, i, j));
		}
		fprintf(stream, "\n");
	}
	fprintf(stream, "]\n\n");
}

Matrix_Op_Result Matrix_Copy(Matrix *src, Matrix *dst) {
	if (src->rows != dst->rows || src->cols != dst->cols) {
		return OP_DIMENSION_MISMATCH;
	}

	for (int i = 0; i < dst->rows; i++) {
		for (int j = 0; j < dst->cols; j++) {
			MAT_ELEM(dst, i, j) = MAT_ELEM(src, i, j);
		}
	}

	return OP_OK;
}

Matrix_Op_Result Matrix_Add(Matrix *m1, Matrix *m2, Matrix *dst) {
	if (m1->rows != m2->rows || m1->cols != m2->cols || m1->rows != dst->rows || m1->cols != dst->cols) {
		return OP_DIMENSION_MISMATCH;
	}

	for (int i = 0; i < m1->rows; i++) {
		for (int j = 0; j < m1->cols; j++) {
			MAT_ELEM(dst, i, j) = MAT_ELEM(m1, i, j) + MAT_ELEM(m2, i, j);
		}
	}

	return OP_OK;
}

Matrix_Op_Result Matrix_Sub(Matrix *m1, Matrix *m2, Matrix *dst) {
	if (m1->rows != m2->rows || m1->cols != m2->cols || m1->rows != dst->rows ||
		m1->cols != dst->cols) {
		return OP_DIMENSION_MISMATCH;
	}

	for (int i = 0; i < m1->rows; i++) {
		for (int j = 0; j < m1->cols; j++) {
			MAT_ELEM(dst, i, j) = MAT_ELEM(m1, i, j) - MAT_ELEM(m2, i, j);
		}
	}

	return OP_OK;
}

static scalar Matrix_Dot_RowCol(Matrix *m1, const int row_m1, Matrix *m2, const int col_m2) {
	scalar res = 0;
	/* Assume that m1 and m2 are both squared and same order */
	for (int i = 0; i < m1->rows; i++) {
		res += MAT_ELEM(m1, row_m1, i) * MAT_ELEM(m2, i, col_m2);
	}

	return res;
}

Matrix_Op_Result Matrix_Multiply(Matrix *m1, Matrix *m2, Matrix *dst) {
	/* Check for m1 and m2 product compatibility */
	if (m1->cols != m2->rows) {
		return OP_DIMENSION_MISMATCH;
	}

	/* (n, m) dot (m, p) ===> (n, p) */
	if (dst->rows != m1->rows || dst->cols != m2->cols) {
		return OP_DIMENSION_MISMATCH;
	}

	for (int i = 0; i < dst->rows; i++) {
		for (int j = 0; j < dst->cols; j++) {
			MAT_ELEM(dst, i, j) = Matrix_Dot_RowCol(m1, i, m2, j);
		}
	}

	return OP_OK;
}

void Matrix_Scalar_Multiply(Matrix *m, scalar s) {
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			MAT_ELEM(m, i, j) = s * MAT_ELEM(m, i, j);
		}
	}
}

Matrix_Op_Result Matrix_Transpose(Matrix *src, Matrix *dst) {
	if (src->rows != dst->rows || src->cols != dst->cols) {
		return OP_DIMENSION_MISMATCH;
	}

	for (int i = 0; i < dst->rows; i++) {
		for (int j = 0; j < dst->cols; j++) {
			MAT_ELEM(dst, i, j) = MAT_ELEM(src, j, i);
		}
	}

	return OP_OK;
}

Matrix_Op_Result Matrix_Trace(Matrix *m, scalar *trace) {
	if (m->rows != m->cols) {
		return OP_DIMENSION_MISMATCH;
	}

	*trace = 0;
	for (int i = 0; i < m->rows; i++) {
		*trace += MAT_ELEM(m, i, i);
	}

	return OP_OK;
}
