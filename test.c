#include "mxl.h"
#include <stdio.h>

int main() {
	Matrix m1, m2, sum, prod;
	Matrix_Identity(&m1, 2, 2);
	Matrix_Identity(&m2, 2, 2);
	Matrix_FPrint(stdout, &m1);
	Matrix_FPrint(stdout, &m2);

	return 0;
}