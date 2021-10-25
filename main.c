/*
 * main.c
 *
 *  Created on: 29 באוג׳ 2018
 *      Author: yuval_000
 */


/*Header File that appear in every file of the project*/
#include "sudokuGeneral.h"

int main(){

	command curCommand;
	char commandBuffer[BUFFER_SIZE];
	BOARD curBoard;
	UR_ELEMENT head;
	UR_LINK curMove;
	init(&curBoard);

	curMove = &head;
	head.next = NULL;

	if(fgets(commandBuffer, BUFFER_SIZE, stdin) == NULL){
		return 0;
	}
	curCommand = getCommandType(commandBuffer, curBoard.m);

	while(curCommand != ext){
		if(curCommand == solve){
			curCommand = is_solve(&curBoard, &head, commandBuffer) ? curCommand : err;
		}
		else if (curCommand == edit){
			curCommand = is_edit(&curBoard, &head, commandBuffer) ? curCommand : err;
		}
		else if (curCommand == print_board){
			printBoard(&curBoard);
		}
		else if (curCommand == mark_errors){
			curCommand = set_mark_errors(commandBuffer, &(curBoard.mrkErr)) ? curCommand : err;
		}
		else if (curCommand == set){
			curCommand = is_set(&curBoard, curMove, commandBuffer) ? set : err;
		}
		else if (curCommand == validate){
			curCommand = is_validated(&curBoard) ? curCommand : err;
		}
		else if (curCommand == generate){
			curCommand = is_generated(&curBoard, curMove, commandBuffer) ? curCommand : err;
		}
		else if (curCommand == undo){
			curCommand = is_undone(&curBoard, curMove, &head) ? curCommand : err;
		}
		else if (curCommand == redo){
			curCommand = is_redone(&curBoard, curMove) ? curCommand : err;
		}
		else if (curCommand == save){
			curCommand = is_saved(&curBoard, commandBuffer) ? curCommand : err;
		}
		else if (curCommand == hint){
			curCommand = is_hinted(&curBoard, commandBuffer) ? curCommand : err;
		}
		else if (curCommand == num_solutions){
			curCommand = is_counted(&curBoard) ? curCommand : err;
		}
		else if (curCommand == autofill){
			curCommand = is_autofilled(&curBoard, curMove) ? curCommand : err;
		}
		else if (curCommand == reset){
			reset_board(&curBoard, &head, curMove);
		}
		if (curCommand == err){
			printf("%s",MSG_INVALID_COMMAND);
		}
		printf("%s", MSG_ENTER_CMD);
		if(fgets(commandBuffer, BUFFER_SIZE, stdin) == NULL){
			curCommand = ext;
		}else{
			curCommand = getCommandType(commandBuffer, curBoard.m);
		}
	}
	/*Delete board*/
	if (curBoard.gameBoard != NULL){
		int i;
		for (i = curBoard.cols * curBoard.rows; i >= 0; i--){
			free((curBoard.gameBoard)[i]);
		}
		free(curBoard.gameBoard);
	}
	printf("%s", MSG_EXT);
	return 0;
}
