#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "external/fpuz.h"
#include "raylib.h"

#define BLK_SIZE     90
#define SPACING      4
#define DEFSIZE      4
#define NUMFONT      30
#define TXTFONT      (SCREENW / 12)
#define SCREENW      ((BLK_SIZE * game.pzl_size) + SPACING)
#define SCREENH      ((BLK_SIZE * game.pzl_size) + TXTFONT)
#define TOIND(A, B)  (A * game.pzl_size + B)

struct Game {
	int moves, win, pzl_size;
	int *board;
	Color background, foreground;
} game;

void
init_game(int size, int light)
{
	game = (struct Game) {
		.moves = 0,.win = 0,.pzl_size = size,.board = NULL,
		.background = BLACK,.foreground = WHITE
	};
	if (light) {
		game.background = WHITE;
		game.foreground = BLACK;
	}

	game.board = (int *)malloc(game.pzl_size * game.pzl_size * sizeof(int));
	fp_set_size(game.pzl_size);
	fp_generate((int (*)[])game.board);
}

void
cleanup(void)
{
	free(game.board);
}

int
check_win(int *a)
{
	int i;
	for (i = 1; i < game.pzl_size * game.pzl_size; i++)
		if (a[i - 1] >= a[i])
			return 0;
	return 1;
}

struct fp_Pos
get_empty_pos(int *a)
{
	int i, j;
	for (i = 0; i < game.pzl_size; i++)
		for (j = 0; j < game.pzl_size; j++)
			if (a[TOIND(i, j)] == game.pzl_size * game.pzl_size)
				return (struct fp_Pos) { i, j };
	return (struct fp_Pos) { -1, -1 };
}

int
move_cell(int *a)
{
	int i, j, abi, abj, x, y;
	struct fp_Pos epos = { -1, -1 };
	Vector2 mp = GetMousePosition();
	x = (int)mp.x % BLK_SIZE;
	y = (int)mp.y % BLK_SIZE;
	if (x > BLK_SIZE - SPACING || x < SPACING || y > BLK_SIZE - SPACING
	    || y < SPACING)
		return 0;

	j = mp.x / BLK_SIZE;
	i = mp.y / BLK_SIZE;
	if (i > game.pzl_size || j >> game.pzl_size)
		return 0;

	epos = get_empty_pos(a);
	abi = fp_abs(epos.i - i);
	abj = fp_abs(epos.j - j);
	if ((abi == 0 && abj == 1)
	    || (abi == 1 && abj == 0)) {
		a[TOIND(epos.i, epos.j)] = a[TOIND(i, j)];
		a[TOIND(i, j)] = game.pzl_size * game.pzl_size;
		return 1;
	}
	return 0;
}

void
draw(void)
{
	int i, j, blkx, blky, w;
	BeginDrawing();
	ClearBackground(game.background);
	if (!(game.win))
		goto draw_board;
	w = MeasureText("You solved the puzzle", TXTFONT) / 2;
	DrawText(TextFormat
		 ("You solved the puzzle\n\nin %d move%c.\n\nClick to play again.",
		  game.moves, (game.moves > 1) ? 's' : ' '), SCREENW / 2 - w,
		 SCREENH / 2 - (TXTFONT * 1.5), TXTFONT, game.foreground);
	goto end;

 draw_board:
	for (i = 0; i < game.pzl_size; i++) {
		for (j = 0; j < game.pzl_size; j++) {
			if (game.board[TOIND(i, j)] ==
			    game.pzl_size * game.pzl_size)
				continue;
			blky = (BLK_SIZE * i) + SPACING;
			blkx = BLK_SIZE * j + SPACING;
			DrawRectangleLines(blkx, blky, BLK_SIZE - SPACING,
					   BLK_SIZE - SPACING, game.foreground);
			w = MeasureText(TextFormat
					("%d", game.board[TOIND(i, j)]),
					NUMFONT) / 2;
			DrawText(TextFormat("%d", game.board[TOIND(i, j)]),
				 blkx + ((BLK_SIZE - SPACING) / 2) - w,
				 blky + ((BLK_SIZE - SPACING) / 2) -
				 NUMFONT / 2, NUMFONT, game.foreground);
		}
	}
	DrawText(TextFormat("Moves: %d\t\tEsc to exit.", game.moves), 2,
		 BLK_SIZE * game.pzl_size, TXTFONT, game.foreground);
 end:
	EndDrawing();
}

int
update_game(void)
{
	if (IsMouseButtonPressed(0)) {
		if (game.win) {
			return 1;
		} else if (move_cell(game.board)) {
			(game.moves)++;
		}
	}
	game.win = check_win(game.board);
	return 0;
}

void
usage(char *c)
{
	printf("usage: %s\n" "-l         light background\n"
	       "-s number  size of board\n", c);
}

int
main(int argc, char *argv[])
{
	int i, sf = DEFSIZE, lf = 0;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-l") == 0) {
			lf = 1;
		} else if (strcmp(argv[i], "-s") == 0) {
			if (argv[i + 1]) {
				sf = atoi(argv[i + 1]);
				sf = (sf == 0) ? DEFSIZE : sf;
				i++;
			}
		} else {
			usage(argv[0]);
			return 1;
		}
	}
	init_game(sf, lf);
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(SCREENW, SCREENH, "rfpuz");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		if (update_game())
			init_game(sf, lf);
		draw();
	}
	cleanup();
	CloseWindow();
	return 0;
}
