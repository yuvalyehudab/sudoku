CC = gcc
OBJS = main.o main_aux.o solver.o parser.o board.o int_stack.o markErrors.o ur_list.o
EXEC = sudoku-console
COMP_FLAG = -ansi -O2 -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

all: sudoku-console

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o main_aux.o: main.c sudokuGeneral.h board.c board.h clist.h SPBufferset.h funcProto.h int_stack.c int_stack.h markErrors.c parser.c text.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c

solver.o: solver.c sudokuGeneral.h funcProto.h int_stack.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
board.o: board.c board.h sudokuGeneral.h funcProto.h
	$(CC) $(COMP_FLAG) -c $*.c
parser.o: parser.c sudokuGeneral.h funcProto.h
	$(CC) $(COMP_FLAG) -c $*.c
int_stack.o: int_stack.c int_stack.h
	$(CC) $(COMP_FLAG) -c $*.c
ur_list.o: ur_list.c ur_list.h
	$(CC) $(COMP_FLAG) -c $*.c
markErrors.o: markErrors.o sudokuGeneral.h funcProto.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f *.o $(EXEC)
