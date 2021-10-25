/*
 * int_stack.h
 *
 *  Created on: 14 בספט׳ 2018
 *      Author: yuval_000
 */

#ifndef SRC_INT_STACK_H_
#define SRC_INT_STACK_H_

#include <stdlib.h>
#include <stdio.h>

typedef int info;

typedef struct elem{
	info f;
	struct elem *next;
} elem;

typedef struct stack{
	int cnt;
	elem *top;
} stack;

void initialize (stack *s);
void push(info f, stack *s);
info pop(stack *s);
int is_empty_stack(stack *s);

#endif /* SRC_INT_STACK_H_ */
