/*
 * parser.c
 *
 *  Created on: 29 באוג׳ 2018
 *      Author: yuval_000
 */


#include "sudokuGeneral.h"

int isRelevantInMode(int index, mode m);

/* Short therefore elegant */
/*Assuming length of string parameter is less than BUFFER_SIZE*/
/*@param: stringFromUser - string to parse from user.
 *@param: mode - mode the board is in. every command is available in different modes.
 *@param: commands_dict - pointer to string of commands. Replaces copy every call.
 *@ret: (command)res - enum of type command */

command getCommandType(char* stringFromUser, mode m){
	/* An array to store the command so we could use without changing */
	char workingCopy[BUFFER_SIZE], commands_dict[COMMANDS_DICT_SIZE];
	char *userCommand, *candidateCommand;
	int res;

	/* Copy the command to the array */
	strcpy(workingCopy, stringFromUser);
	/* Extract the command from the array */
	userCommand = strtok(workingCopy, COM_SEP);
	/**/
	strcpy(commands_dict, COMMANDS_DICT);

	/* Blank line or white spaces line*/
	if ((userCommand == NULL) || strlen(userCommand) == 0){
		return ignore;
	}

	candidateCommand = strtok(strstr(commands_dict, userCommand),COM_SEP);
	if (candidateCommand != NULL && strcmp(candidateCommand, userCommand) == 0){
		res = candidateCommand - commands_dict;
		if (isRelevantInMode(res, m)){
			return (command)res;
		}
	}

	return err;
}

int isRelevantInMode(int index, mode m){
	if (index <= 11){
		return 1;
	}
	if (m == Init){
		return 0;
	}
	if (index <= 70){
		return 1;
	}
	if (m == Solve){
		if (index <= 100){
			return 1;
		}
	}
	/*mode == Edit*/
	if (index > 100){
		return 1;
	}
	return 0;
}

char *getFileName(char* stringFromUser){
	/*We need the second token
	 * we will use strtok twice*/
	strtok(stringFromUser, COM_SEP);
	return strtok(NULL, COM_SEP);
}

int getSetParams(char *userString, int *paramArr){
	int i;
	strtok(userString, COM_SEP);
	userString = strtok(NULL, COM_SEP);
	for (i = 0; i < 3; i++){
		paramArr[i] = atoi(userString);
		userString = strtok(NULL, COM_SEP);
		if (userString == NULL){
			return i;
		}
	}
	return i;
}

int token2Num(char *token, int max){
	char *p;
	int res;
	p = token;
	res = 0;
	while ((*p) == '0'){
		p++;
	}
	while (p != NULL){
		if (!isdigit(*p)){
			return -1;
		}
		res = res*10 + (*p - '0');
		if (res > max){
			return -1;
		}
	}
	return res;
}

int getGenParams(int *X, int *Y, char *com, int b_size){
	char *x_str, *y_str;
	strtok(com, COM_SEP);
	x_str = strtok(NULL, COM_SEP);
	if (x_str == NULL){
		return 0;
	}
	*X = token2Num(x_str, b_size);/*too much work...*/
	if (*X < 0){
		return 0;
	}
	y_str = strtok(NULL, COM_SEP);
	if (y_str == NULL){
		return 0;
	}
	*Y = token2Num(y_str, b_size);
	return (*X >= 0) + (*Y >= 0);
}

int get_X_Y_params(char *command, int *col, int *row, int b_size){
	char *col_str, *row_str;
	strtok(command, COM_SEP);
	col_str = strtok(NULL, COM_SEP);
	if (col_str == NULL){
		return 0;
	}
	*col = token2Num(col_str, b_size);/*too much work...*/
	if (*col < 0){
		return 0;
	}
	row_str = strtok(NULL, COM_SEP);
	if (row_str == NULL){
		return 0;
	}
	*row = token2Num(row_str, b_size);
	return (*col >= 0) + (*row >= 0);
}

void read_mat_from_file(FILE *fp, BOARD *b){
	int c, col, row, val, bSize;
	bSize = b ->rows * b ->cols;

	c = fgetc(fp);
	col=0;row=0;
	while(c!=EOF){
		while (isspace(c)){
			c = fgetc(fp);
		}
		val=0;
		while (isdigit(c)){
			val = val*10 + (c - '0');
			if (val > bSize){
				printf("%s", "Error:  file format");
				exit(-1);
			}
			c=fgetc(fp);
		}
		if (c == '.'){
			if (val == 0){
				printf("%s", "error in file! 0 can't be fixed.");
				exit(-1);
			}
			if (b ->m == Solve){
				setFixed(b, col, row);
			}
		}
		b ->gameBoard[col][row] = val;
		val =0;
		col = (col + 1)%bSize;
		row = (row + 1)/bSize;
		c=fgetc(fp);
	}
}
