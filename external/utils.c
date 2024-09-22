#include <stdio.h>
#include <stdlib.h>

#include "fpuz.h"

int FP_SIZE;

void
fp_set_size(int s)
{
	FP_SIZE = s;
}

int
fp_abs(int n)
{
	return (n > 0) ? n : -n;
}

void
fp_swap_cell(struct fp_Cell *a, struct fp_Cell *b)
{
	struct fp_Cell tmp = *a;
	*a = *b;
	*b = tmp;
}

void
fp_swap_pos(struct fp_Pos *a, struct fp_Pos *b)
{
	struct fp_Pos tmp = *a;
	*a = *b;
	*b = tmp;
}

void
fp_print(int a[FP_SIZE][FP_SIZE])
{
	int i, j;
	for (i = 0; i < FP_SIZE; i++) {
		for (j = 0; j < FP_SIZE; j++) {
			printf("%d\t", a[i][j]);
		}
		printf("\n");
	}
}

int
fp_is_solvable(int a[FP_SIZE][FP_SIZE])
{
	int i, j, inversions = fp_get_inversions((int *)a), epos = 0,
	    s = FP_SIZE * FP_SIZE;
	for (i = 0; i < FP_SIZE; i++) {
		for (j = 0; j < FP_SIZE; j++) {
			if (a[i][j] == s) {
				epos = FP_SIZE - i;
				break;
			}
		}
	}
	if (FP_SIZE & 1)
		return !(inversions & 1);
	if (epos & 1)
		return !(inversions & 1);
	else
		return inversions & 1;
}

int
fp_get_inversions(int a[])
{
	int i, j, inversions = 0, s = FP_SIZE * FP_SIZE;
	for (i = 0; i < FP_SIZE * FP_SIZE - 1; i++) {
		for (j = i + 1; j < FP_SIZE * FP_SIZE; j++) {
			if (a[j] != s && a[i] != s && a[i] > a[j])
				inversions++;
		}
	}
	return inversions;
}
