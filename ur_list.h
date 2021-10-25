/*
 * URList.h
 *
 *  Created on: 12 בספט׳ 2018
 *      Author: yuval_000
 */

#ifndef SRC_UR_LIST_H_
#define SRC_UR_LIST_H_

#include "clist.h"

typedef C_LINK DATA;

typedef struct URlist {
	DATA d;
	struct URlist *prev;
	struct URlist *next;
}UR_ELEMENT;

typedef UR_ELEMENT *UR_LINK;

/*in ur_list.c*/
void clear_ur_list(UR_LINK from);
C_LINK create_c_element(int from, int to, int col, int row);
UR_LINK create_ur_element(UR_LINK cur, DATA d);
void init_ur_list(UR_LINK h);

#endif /* SRC_UR_LIST_H_ */
