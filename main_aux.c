/*
 * mainAux.c
 *
 *  Created on: 4 בספט׳ 2018
 *      Author: yuval_000
 */


#include "sudokuGeneral.h"

void init(BOARD *b){
	SP_BUFF_SET();
	srand(time(NULL));
	b->m = Init;
	b->mrkErr = 1;
	b->gameBoard = NULL;
	printf(GREETING);
	printf("%s",MSG_ENTER_CMD);
}

int isInRange(int var, int bottom, int top){
	if (var < bottom){
		return 0;
	}
	if (var > top){
		return 0;
	}
	return 1;/*var >= bottom && var <= top*/
}

int *chooseXCells(int X, int max){
	int *res, *indices;
	int i, index;
	/*choose random indices between 0 (include) and max (not include)*/
	indices = (int *)malloc(max * sizeof(int));
	exit_malloc_failed(indices == NULL);
	/*place for indices*/
	res = (int *)malloc(X * sizeof(int));
	exit_malloc_failed(res == NULL);
	for (i = 0; i < max; i++){
		indices[i] = i;
	}
	for (i = 0; i < X; i++){
		index = (rand() % (max - i)) + i;
		res[i] = indices[index];
		indices[index] = indices[i];
	}
	return res;
}

int rndNum(int *arr, int size){
	return arr[rand() % size];
}

/*kind of assert*/
void exit_malloc_failed(int cond){
	if (cond){
		printf("%s", "malloc failed");
		exit(-1);
	}
}

void empty_board(BOARD *b){
	int i, j;
	for (i = 0; i < (b ->cols) * (b ->rows); i++){
		for (j = 0; j < (b ->cols) * (b ->rows); j++){
			(b ->gameBoard)[i][j] = 0;
		}
	}
}
/*bool*/

int is_solve(BOARD *b, UR_LINK head, char *command){
	int success, cols, rows;
	char *fileName;
	FILE *fp;
	fileName = getFileName(command);
	if (fileName == NULL || *fileName == 0){
		printf("%s",MSG_INVALID_COMMAND);
		return 0;
	}
	fp = fopen(fileName, "r");
	if (fp == NULL){
		printf("%s", MSG_FILE_ERR_SOLVE);
		return 0;
	}

	success = fscanf(fp, "%d %d\n", &rows, &cols);
	if (success != 2){
		printf("err");
		return 0;
	}
	b ->cols = cols; b ->rows = rows;
	delete_game_board(b);
	b ->gameBoard = new_mat((b ->rows) * (b ->cols));
	clear_fixed(b);
	read_mat_from_file(fp, b);
	fclose(fp);
	b ->m = Solve;
	init_ur_list(head);
	printBoard(b);
	return 1;
}

int is_edit(BOARD *b, UR_LINK head, char *command){
	char *fileName;
	fileName = getFileName(command);
	if (fileName == NULL || *fileName == 0){
		/*new 3*3 mat*/

		delete_game_board(b);
		b ->cols = 3;
		b ->rows = 3;
		b ->gameBoard = new_mat((b ->cols) * (b ->rows));
		empty_board(b);
	}else{
		FILE *fp;
		int success, cols, rows, bSize;
		fp = fopen(fileName, "r");

		if (fp == NULL){
			printf("%s", MSG_FILE_ERR_EDIT);
			return 0;
		}

		success = fscanf(fp, "%d %d\n", &rows, &cols);
		if (success != 2){
			printf("%s", "error");
			return 0;
		}
		delete_game_board(b);
		b ->cols = cols; b->rows = rows;
		bSize = rows * cols;
		b ->gameBoard = new_mat(bSize);
		read_mat_from_file(fp, b);
		fclose(fp);
	}
	b ->m = Edit;
	init_ur_list(head);
	printBoard(b);
	return 1;
}

int is_set(BOARD *b, UR_LINK cur, char *command){
	int i, val, col, row, params[3];
	char *token;
	C_LINK tmp;
	strtok(command,COM_SEP);
	for (i = 0; i < 3; i++){
		token = strtok(NULL, " \t\r\n");
		params[i] = token2Num(token, b->rows * b->cols);
		if (params[i] < 0){
			printf(MSG_VAL_RANGE, SET_MIN, (b->rows * b->cols));
			return 0;
		}
	}
	if (params[0] == 0 || params[1] == 0){
		printf(MSG_VAL_RANGE, SET_MIN, (b->rows * b->cols));
		return 0;
	}
	col = params[0] - 1;
	row = params[1] - 1;
	if (isFixedCell(col, row, b)){
		printf("%s", MSG_CELL_FIXED);
		return 0;
	}
	clear_ur_list(cur);
	val = (b->gameBoard)[col][row];
	tmp = create_c_element(val, params[2], col, row);
	cur ->next = create_ur_element(cur, tmp);
	cur = cur->next;

	(b->gameBoard)[col][row] = params[2];
	printBoard(b);
	if (boardFull(b)){
		if (isErrBoard(b)){
			printf("%s",MSG_PUZZLE_ERR);
		}else{
			printf("%s", MSG_PUZZLE_SOLVED);
			b->m = Init;
		}
	}
	return 1;
}

/*boolean*/
int is_validated(BOARD *b){
	int **mat, i;
	if (isErrBoard(b)){
		printf("%s", MSG_ERR_VALS);
		return 0;
	}
	mat = copy_mat(b->gameBoard, (b->cols * b->rows));
	if (solve_ILP(mat, b->cols, b->rows)){
		printf("%s", MSG_BOARD_VALID);
	}else{
		printf("%s", MSG_BOARD_INVALID);
	}
	for (i = 0; i < (b->cols * b->rows); i++){
		free(mat[i]);
	}
	free(mat);
	return 1;
}

int try_to_fill(BOARD *b, int *arr, int arr_size){
	int i, col, row, ops_size, *ops;
	for (i = 0; i < arr_size; i++){
		col = arr[i]%(b->rows * b->cols);
		row = arr[i]/(b->rows * b->cols);
		ops = opsForCell(b, &ops_size, col, row);
		if (ops_size == 0){
			free(ops);
			return 0;
		}
		b->gameBoard[col][row] = rndNum(ops,ops_size);
		free(ops);
	}
	return 1;
}

int is_generated(BOARD *b, UR_LINK cur, char *command){
	int x, y, numOfParam, max, side;
	int i, j, col, row, solve_err, *rnd_cells;
	C_LINK tmp_c;
	UR_LINK tmp_ur;

	side = b->cols * b->rows;
	max = side * side;
	numOfParam = get_X_Y_params(command, &x, &y, max);
	if (numOfParam != 2){
		return 0;
	}
	if (!isInRange(x, 0, max) || !isInRange(y, 0, max)){
		printf(MSG_GEN_RANGE, max);
		return 0;
	}
	if (!isEmpty(b)){
		printf("%s", MSG_NOT_EMPTY);
		return 0;
	}
	/*1000*/
	for (j = 0; j < 1000; j++) {
		rnd_cells = chooseXCells(x,max);
		if (try_to_fill(b, rnd_cells, x) == 0){
			empty_board(b);
			continue;
		}
		free(rnd_cells);
		solve_err = ILP_fill(b);
		if (solve_err == 1){
			j--;
			break;
		}
	}
	/*end 1000*/
	if (j == 1000){
		printf("%s", MSG_GEN_FAIL);
		return 0;
	}
	rnd_cells = chooseXCells(y, max);
	for (i = 0; i < y; i++){
		col = rnd_cells[i] % (b->cols * b->rows);
		row = rnd_cells[i] / (b->cols * b->rows);
		b->gameBoard[col][row] = 0;
	}
		/*add list elem*/
	tmp_c = NULL;
	tmp_ur = NULL;
	for (i = 0; i < side; i++){
		for (j = 0; j < side; j++){
			if (b->gameBoard[j][i] != 0){
				tmp_c = create_c_element(0, b->gameBoard[j][i], j, i);
				if (tmp_ur == NULL){
					tmp_ur = create_ur_element(cur, tmp_c);
				}else{
					tmp_ur->d->next = tmp_c;
				}
			}
		}
	}
	if (tmp_ur != NULL){
		cur->next = tmp_ur;
		cur = cur->next;
	}
	printBoard(b);
	return 1;
}

void undo_move(BOARD *b, UR_LINK cur){
	DATA tmp;
	tmp = cur ->d;
	while (tmp != NULL){
		b ->gameBoard[tmp ->c.x][tmp ->c.y] = tmp ->c.from;
		tmp = tmp ->next;
	}
}
void print_undone(UR_LINK cur){
	DATA tmp;
	tmp = cur ->d;
	while (tmp != NULL){
		printf(MSG_MOVE_UNDO, tmp ->c.x, tmp ->c.y, tmp ->c.to, tmp ->c.from);
		tmp = tmp ->next;
	}
}
/*boolean*/
/* placed in sudoku_general*/
int is_undone(BOARD *b, UR_LINK cur, UR_LINK head){

	if (cur == head){
		printf("%s", MSG_ERR_UNDO);
		return 0;
	}
	undo_move(b, cur);
	printBoard(b);
	print_undone(cur);

	cur = cur ->prev;
	return 1;
}
void redo_move(BOARD *b, UR_LINK cur){
	DATA tmp;
	tmp = cur ->d;
	while (tmp != NULL){
		b ->gameBoard[tmp ->c.x][tmp ->c.y] = tmp ->c.to;
		tmp = tmp->next;
	}
}
void print_redone(UR_LINK cur){
	DATA tmp;
	tmp = cur ->d;
	while (tmp != NULL){
		printf(MSG_MOVE_REDO, tmp ->c.x, tmp ->c.y, tmp ->c.from, tmp ->c.to);
		tmp = tmp->next;
	}
}
/*boolean*/
/* placed in sudoku_general*/
int is_redone(BOARD *b, UR_LINK cur){

	if (cur ->next == NULL){
		printf("%s", MSG_ERR_REDO);
		return 0;
	}
	cur = cur ->next;
	redo_move(b, cur);
	printBoard(b);

	print_redone(cur);
	return 1;
}

int is_saved(BOARD *b, char *command){
	char *f_name;
	int i, j, side; FILE *fp;
	f_name = getFileName(command);

	if (b->m == Edit){
		if (isErrBoard(b)){
			printf("%s", MSG_ERR_VALS);
			return 0;
		}
		if (validateBoard(b) == 0){
			printf("%s", MSG_VAL_FAIL);
			return 0;
		}
	}
	/*validation succeeded, need to be saved*/
	/*try to open file*/
	fp = fopen(f_name,"w");
	if (fp == NULL){
		printf("%s", MSG_FILE_SAVE);
		return 0;
	}
	side = b->rows * b->cols;
	fprintf(fp, "%d %d\n",b->rows, b->cols);
	for (i = 0; i < side; i++){
		for (j = 0; j < side; j++){
			fprintf(fp, "%d", b->gameBoard[j][i]);
			if (b ->m == Edit && b->gameBoard[j][i] != 0){
				fprintf(fp, "%c", '.');
			}
			if (b ->m == Solve && isFixedCell(j, i, b)){
				fprintf(fp, "%c", '.');
			}
			fprintf(fp, "%c", j == side - 1 ? '\n' : ' ');
		}
	}
	printf(MSG_SAVED_TO, f_name);
	fclose(fp);
	return 1;
}

/*boolean*/
/*sudoku_general*/
int is_hinted(BOARD *b, char *command){
	int param_amt, col, row, block_size;
	block_size = (b ->cols) * (b ->rows);
	col = -1; row = -1;
	param_amt = get_X_Y_params(command, &col, &row, block_size);
	col--; row--;
	if (param_amt < 2){
		printf(MSG_VAL_RANGE, HINT_MIN, block_size);
	}else if (isErrBoard(b)){
		printf("%s", MSG_ERR_VALS);
	}else if (isFixedCell(col, row, b)){
		printf("%s", MSG_CELL_FIXED);
	}else if ((b ->gameBoard)[col][row] != 0){
		printf("%s", MSG_CELL_FULL);
	}else{
		int val;
		val = get_hint(b, col, row);
		if (val == 0){
			printf("%s", MSG_BOARD_INVALID);
		}else{
			printf(MSG_HINT, val);
			return 1;
		}
	}
	return 0;
}

/*boolean*/
/*sudoku_general*/
int is_counted(BOARD *b){

	int sol_amt;
	sol_amt = 0;

	if (isErrBoard(b)){
		printf("%s", MSG_ERR_VALS);
		return 0;
	}

	sol_amt = count_sols(b);
	printf(MSG_SOL_AMT, sol_amt);
	if (sol_amt == 1){
		printf("%s", MSG_GOOD_BOARD);
	}else if (sol_amt > 1){
		printf("%s", MSG_MORE_SOLS);
	}
	return 1;
}

int is_autofilled(BOARD *b, UR_LINK cur){
	C_LINK tmp_c_link;
	if (isErrBoard(b)){
		printf("%s", MSG_ERR_VALS);
		return 0;
	}
	/*make create and delete list!*/

	tmp_c_link = filled_cells(b);
	if (tmp_c_link == NULL){
		printBoard(b);
		return 1;
	}
	clear_ur_list(cur);
	cur ->next = create_ur_element(cur, tmp_c_link);

	cur = cur ->next;

	printBoard(b);

	return 1;
}

void reset_board(BOARD *b, UR_LINK head, UR_LINK cur){
	while (cur != head){
		undo_move(b, cur);
		cur = cur ->prev;
	}
	clear_ur_list(head);
	clear_c_list(head ->d);
	head ->next = NULL;
	head ->d = NULL;

	printf("%s", MSG_RESET);
}

int get_hint(BOARD *b, int col, int row){
	int **mat, b_cols, b_rows, error, res, i;
	b_cols = b ->cols;
	b_rows = b ->rows;
	mat = copy_mat(b ->gameBoard, (b_cols * b_rows));
	error = solve_ILP(mat, b_cols, b_rows);
	res = mat[col][row];
	for (i = 0; i < b_cols*b_rows; i++){
		free(mat[i]);
	}
	free(mat);
	return (error ? 0 : res);
}
/*func_proto.h*/
int **new_mat(int side){
	int i, **mat;
	mat = (int **)malloc(side * sizeof(int *));
	exit_malloc_failed(mat == NULL);
	for (i = 0; i < side; i++){
		mat[i] = (int *)malloc(side * sizeof(int));
		exit_malloc_failed(mat[i] == NULL);
		}
	return mat;
}

int **copy_mat(int **old_mat, int side){
	int i, j, **mat;
	mat = new_mat(side);
	/*now make a copy*/
	for (i = 0; i < side; i++){
		for (j = 0; j < side; j++){
			mat[i][j] = old_mat[i][j];
		}
	}
	return mat;
}
