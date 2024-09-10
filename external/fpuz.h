#ifndef FPUZ_H
#define FPUZ_H

extern int FP_SIZE;

struct fp_Cell {
	int num, done;
};

struct fp_Pos {
	int i, j;
};

struct fp_Answer {
	int len, cap;
	struct fp_Pos *cells_move;
	int *empty_move;
};

enum direction {
	Up = 1,
	Down = -1,
	Right = 2,
	Left = -2,
	Stop = 0
};

/* utils */

int fp_abs(int n);
void fp_swap_cell(struct fp_Cell *a, struct fp_Cell *b);
void fp_swap_pos(struct fp_Pos *a, struct fp_Pos *b);
void fp_print(int size, int a[size][size]);
int fp_is_solvable(int a[FP_SIZE][FP_SIZE]);
int fp_get_inversions(int a[]);

/* generator */

void fp_generate(int size, int a[size][size]);
void fp_shuffle(int a[FP_SIZE][FP_SIZE]);

/* solver */

struct fp_Answer *fp_solve(int size, int p[size][size]);

int fp_solve_puzzle(struct fp_Cell a[FP_SIZE][FP_SIZE],
		    struct fp_Pos start, struct fp_Pos *epos);

int fp_find_path(struct fp_Cell a[FP_SIZE][FP_SIZE],
		 enum direction drc[FP_SIZE * FP_SIZE], int drcindx,
		 struct fp_Pos tpos, struct fp_Pos epos);

void fp_move_empty_cell(struct fp_Cell a[FP_SIZE][FP_SIZE],
			struct fp_Pos target_pos, struct fp_Pos *epos);

void fp_move_cell(struct fp_Cell a[FP_SIZE][FP_SIZE], int n,
		  struct fp_Pos dst, struct fp_Pos *epos, int done);

enum direction fp_get_direction(struct fp_Pos p, struct fp_Pos nextp);

/* answer */

struct fp_Answer *fp_new_answer();
int fp_allocate_ans(struct fp_Answer *a);
int fp_add_moves(struct fp_Answer *a, struct fp_Pos p, int d);
void fp_clean_answer(struct fp_Answer *a);

#endif				// FPUZ_H
