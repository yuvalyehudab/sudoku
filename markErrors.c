/*
 * markErrors.c
 *
 *  Created on: 8 בספט׳ 2018
 *      Author: yuval_000
 */

#include "sudokuGeneral.h"

int getMarkParam(char *command){
	/**/

	strtok(command, COM_SEP);
	strtok(NULL, COM_SEP);

	if ((*command) == '0' || (*command) == '1'){
		if (*(command+1) == 0 || isspace(*(command+1))){
			return (*command) - '0';
		}
	}
	return -1;
}

int set_mark_errors(char *command, int *mark_ptr){

	int markParam;

	markParam = getMarkParam(command);
	if (markParam >= 0){
		*mark_ptr = markParam;
		return 1;
	}

	printf("%s", MSG_MARK_INVALID_PARAM);
	return 0;
}
