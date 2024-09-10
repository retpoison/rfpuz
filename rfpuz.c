#include <string.h>
#include "external/fpuz.h"
#include "raylib.h"

#define BLK_SIZE 90
#define PZL_SIZE 4
#define FONTSIZE 30
#define SPACING  4
#define SCREENW  ((BLK_SIZE * PZL_SIZE) + SPACING)
#define SCREENH  ((BLK_SIZE * PZL_SIZE) + 50)

struct Game {
	int moves, win;
	int board[PZL_SIZE][PZL_SIZE];
	int lboard[PZL_SIZE * PZL_SIZE];
	Color Background, Foreground;
};

struct Game
init_game(void)
{
	struct Game g = { 0, 0, {{0}}, {0}, BLACK, WHITE };
	fp_generate(PZL_SIZE, g.board);
	return g;
}

int
check_win(int a[PZL_SIZE * PZL_SIZE])
{
	int i;
	for (i = 1; i < PZL_SIZE * PZL_SIZE; i++)
		if (a[i - 1] >= a[i])
			return 0;
	return 1;
}

struct fp_Pos
get_empty_pos(int a[PZL_SIZE][PZL_SIZE])
{
	int i, j;
	for (i = 0; i < PZL_SIZE; i++)
		for (j = 0; j < PZL_SIZE; j++)
			if (a[i][j] == PZL_SIZE * PZL_SIZE)
				return (struct fp_Pos) { i, j };
	return (struct fp_Pos) { -1, -1 };
}

int
move_cell(int a[PZL_SIZE][PZL_SIZE])
{
	int i, j, abi, abj, x, y;
	struct fp_Pos epos = { -1, -1 };
	Vector2 mp = GetMousePosition();
	x = (int)mp.x % BLK_SIZE;
	y = (int)mp.y % BLK_SIZE;
	if (x > BLK_SIZE - SPACING || x < SPACING ||
	    y > BLK_SIZE - SPACING || y < SPACING)
		return 0;

	j = mp.x / BLK_SIZE;
	i = mp.y / BLK_SIZE;
	if (i > PZL_SIZE || j >> PZL_SIZE)
		return 0;

	epos = get_empty_pos(a);
	abi = fp_abs(epos.i - i);
	abj = fp_abs(epos.j - j);
	if ((abi == 0 && abj == 1)
	    || (abi == 1 && abj == 0)) {
		a[epos.i][epos.j] = a[i][j];
		a[i][j] = PZL_SIZE * PZL_SIZE;
		return 1;
	}
	return 0;
}

void
draw(struct Game *g)
{
	int i, j, blkx, blky, w;
	BeginDrawing();
	ClearBackground(g->Background);
	if (!(g->win))
		goto draw_board;
	w = MeasureText("You solved the puzzle", FONTSIZE) / 2;
	DrawText(TextFormat
		 ("You solved the puzzle\n\nin %d move%c.\n\nClick to play again.",
		  g->moves, (g->moves > 1) ? 's' : ' '), SCREENW / 2 - w,
		 SCREENH / 2 - (FONTSIZE * 1.5), FONTSIZE, g->Foreground);
	goto end;

 draw_board:
	for (i = 0; i < PZL_SIZE; i++) {
		for (j = 0; j < PZL_SIZE; j++) {
			g->lboard[i * PZL_SIZE + j] = g->board[i][j];
			if (g->board[i][j] == PZL_SIZE * PZL_SIZE)
				continue;
			blky = (BLK_SIZE * i) + SPACING;
			blkx = BLK_SIZE * j + SPACING;
			DrawRectangleLines(blkx, blky,
					   BLK_SIZE - SPACING,
					   BLK_SIZE - SPACING, g->Foreground);
			w = MeasureText(TextFormat("%d", g->board[i][j]),
					FONTSIZE) / 2;
			DrawText(TextFormat("%d", g->board[i][j]),
				 blkx + ((BLK_SIZE - SPACING) / 2) - w,
				 blky + ((BLK_SIZE - SPACING) / 2) -
				 FONTSIZE / 2, FONTSIZE, g->Foreground);
		}
	}
	DrawText(TextFormat("Moves: %d\t\tEsc to exit.", g->moves),
		 2, BLK_SIZE * PZL_SIZE, FONTSIZE, g->Foreground);
 end:
	EndDrawing();
}

void
update_game(struct Game *g)
{
	if (IsMouseButtonPressed(0)) {
		if (g->win) {
			*g = init_game();
		} else if (move_cell(g->board)) {
			(g->moves)++;
		}
	}
	g->win = check_win(g->lboard);
}

int
main(int argc, char *argv[])
{
	struct Game game = init_game();
	if (argc > 1) {
		if (strcmp(argv[1], "-l") == 0
		    || strcmp(argv[1], "--light") == 0) {
			game.Background = WHITE;
			game.Foreground = BLACK;
		}
	}

	SetTraceLogLevel(LOG_WARNING);
	InitWindow(SCREENW, SCREENH, "rfpuz");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		update_game(&game);
		draw(&game);
	}
	CloseWindow();
	return 0;
}
