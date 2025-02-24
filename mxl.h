#ifndef __MXL_H
#define __MXL_H

typedef float scalar;

typedef struct {
	int rows;
	int cols;
	scalar *raw;
} Matrix;

#define MAT_ELEM(m, i, j) (m.raw[(i) * m.cols + (j)])

typedef enum {
	OP_OK,
	OP_NOT_OK,
	OP_DIMENSION_MISMATCH,
	OP_SINGULAR_MATRIX
} Matrix_Op_Result;

void Matrix_Init(Matrix *matrix, const int r, const int c);
void Matrix_Free(Matrix *matrix);

Matrix_Op_Result Matrix_Euclidean_Norm(Matrix *vector, scalar *norm);

void Matrix_Print(Matrix *m);
void Matrix_Copy(Matrix *src, Matrix *dst);
void Matrix_Identity(Matrix *m);
void Matrix_Zeros(Matrix *m);
void Matrix_Add(Matrix *m1, Matrix *m2);
void Matrix_Sub(Matrix *m1, Matrix *m2);
Matrix_Op_Result Matrix_Multiply(Matrix *m1, Matrix *m2);
void Matrix_Scalar_Multiply(Matrix *m, scalar s);
void Matrix_Transpose(Matrix *src, Matrix *dst);
Matrix_Op_Result Matrix_Rank(Matrix *m, int *rank);
Matrix_Op_Result Matrix_Determinant(Matrix *m, scalar *det);
Matrix_Op_Result Matrix_Invert(Matrix *src, Matrix *dst);
Matrix_Op_Result Matrix_Trace(Matrix *src, scalar *trace);

#endif /* __MXL_H */
