/*
 * sudokuGeneral.h
 *
 *  Created on: 4 בספט׳ 2018
 *      Author: yuval_000
 */

#ifndef SUDOKUGENERAL_H_
#define SUDOKUGENERAL_H_

/* includes */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "SPBufferset.h"
#include "board.h"
#include "text.h"
#include "funcProto.h"
#include "ur_list.h"

/* defines */
#define COM_SEP " \t\r\n"
#define BUFFER_SIZE 256
#define COMMANDS_DICT_SIZE 128

/* enums */
typedef enum {
	ext = 0,
	solve = 5,
	edit = 11,
	print_board = 16,
	set = 28,
	validate = 32,
	undo = 41,
	redo = 46,
	save = 51,
	num_solutions = 55,
	reset = 69,
	mark_errors = 75,
	autofill = 87,
	hint = 96,
	generate = 101,
	err,
	ignore
} command;

command getCommandType(char* stringFromUser, mode m);
int *opsForCell(BOARD *board, int *opsNum, int col, int row);

int valInNeighbors(BOARD *board, int val, int col, int row);
int boardFull(BOARD *board);

int boardSolved(BOARD *board);
int token2Num(char *token, int max);
int isFixedCell(int col, int row, BOARD *board);

/*void fillObviusCells(BOARD *board, UR_LINK newMove);*/
/*board.c*/
C_ELEMENT *filled_cells(BOARD *b);

int validateBoard(BOARD *board);
/*parser*/
void read_mat_from_file(FILE *fp, BOARD *b);
/*board*/
void clear_fixed(BOARD *b);
void setFixed(BOARD *board, int col, int row);
void delete_game_board(BOARD *b);

int get_X_Y_params(char *command, int *col, int *row, int b_size);
int get_hint(BOARD *b, int col, int row);

int isInRange(int var, int bottom, int top);
int first_empty_cell(BOARD *b, int start);

int isEmpty(BOARD *b);
int isErrCell(int col, int row, BOARD *board);
int *chooseXCells(int X, int max);

void exit_malloc_failed(int cond);
int rndNum(int *arr, int size);

int ILP_fill(BOARD *b);
int solve_ILP(int **mat, int b_cols,int b_rows);

int count_sols(BOARD *b);

int is_solve(BOARD *b, UR_LINK head, char *command);
int is_edit(BOARD *b, UR_LINK head, char *command);
int is_set(BOARD *b, UR_LINK cur, char *command);

int is_generated(BOARD *b, UR_LINK cur, char *command);
int is_validated(BOARD *b);

int is_undone(BOARD *b, UR_LINK cur, UR_LINK head);
int is_redone(BOARD *b, UR_LINK cur);
int is_saved(BOARD *b, char *command);

int is_hinted(BOARD *b, char *command);
int is_counted(BOARD *b);
int is_autofilled(BOARD *b, UR_LINK cur);

void reset_board(BOARD *b, UR_LINK head, UR_LINK cur);

#endif /* SUDOKUGENERAL_H_ */
