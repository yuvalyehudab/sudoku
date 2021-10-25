/*
 * int_stack.c
 *
 *  Created on: 14 בספט׳ 2018
 *      Author: yuval_000
 */

#include "int_stack.h"

void initialize (stack *s){
	s ->cnt = 0;
	s ->top = NULL;
}

void push(info f, stack *s){
	elem *p;
	p = (elem *)malloc(sizeof(elem));
	if (p == NULL){
		printf("%s", "malloc failed, sorry!");
		exit(-1);
	}
	p ->f = f;
	p ->next = s ->top;
	s ->top = p;
	s ->cnt++;
}

info pop(stack *s){
	info f;
	elem *p;

	f = s ->top ->f;
	p = s ->top;
	s ->top = s ->top ->next;
	s ->cnt--;
	free(p);
	return f;
}
int is_empty_stack(stack *s){
	return (s ->cnt == 0);
}
