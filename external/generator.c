#include <stdlib.h>
#include <time.h>

#include "fpuz.h"

int FP_SIZE;

void
fp_generate(int size, int a[size][size])
{
	FP_SIZE = size;
	int i, j;
	for (i = 0; i < FP_SIZE; i++)
		for (j = 0; j < FP_SIZE; j++)
			a[i][j] = i * FP_SIZE + j + 1;

	srand((rand() % 10) * time(NULL));
	while (1) {
		fp_shuffle(a);
		if (fp_is_solvable(a))
			break;
	}
}

void
fp_shuffle(int a[FP_SIZE][FP_SIZE])
{
	int n = rand() % 100 + 100;

	int r1, r2, i1, j1, i2, j2, s = FP_SIZE * FP_SIZE, tmp;
	for (int _ = 0; _ < n; _++) {
		r1 = rand() % s;
		r2 = rand() % s;
		i1 = r1 / FP_SIZE;
		j1 = r1 % FP_SIZE;
		i2 = r2 / FP_SIZE;
		j2 = r2 % FP_SIZE;
		tmp = a[i1][j1];
		a[i1][j1] = a[i2][j2];
		a[i2][j2] = tmp;
	}
}
