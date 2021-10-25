/*
 * ur_list.c
 *
 *  Created on: 16 בספט׳ 2018
 *      Author: yuval_000
 */

#include "ur_list.h"
/*should make : create + clear*/
void clear_c_list(C_LINK from){
	C_LINK tmp;
	while (from != NULL){
		tmp = from;
		from = from ->next;
		free(tmp);
	}
}

void clear_ur_list(UR_LINK from){
	UR_LINK tmp;
	while (from ->next != NULL){
		tmp = from ->next;
		from ->next = (from ->next) ->next;
		clear_c_list(tmp ->d);
		free(tmp);
	}
}

C_LINK create_c_element(int from, int to, int col, int row){
	C_LINK res;
	res = (C_LINK)malloc(sizeof(C_ELEMENT));

	if (res == NULL){
		printf("%s", "malloc failed, bye");
		exit(-1);
	}

	res ->next = NULL;
	res ->c.from = from;
	res ->c.to = to;
	res ->c.x = col;
	res ->c.y = row;
	return res;
}

UR_LINK create_ur_element(UR_LINK cur, DATA d){
	UR_LINK res;
	res = (UR_LINK)malloc(sizeof(UR_ELEMENT));
	if (res == NULL){
		printf("%s", "malloc failed, bye");
		exit(-1);
	}

	res ->d = d;
	res ->next = NULL;
	res ->prev = cur;
	return res;
}

void init_ur_list(UR_LINK h){
	clear_ur_list(h);
	h ->next = NULL;
	clear_c_list(h ->d);
	h->d = NULL;
}
