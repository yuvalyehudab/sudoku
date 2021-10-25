/*
 * solver.c
 *
 *  Created on: 14 בספט׳ 2018
 *      Author: yuval_000
 */
#include "sudokuGeneral.h"
#include "int_stack.h"
#include "gurobi_c.h"

int set_next_val(int cell, BOARD *b);


/*should be written on nova only*/
int ILP_fill(BOARD *b){
	return solve_ILP(b->gameBoard, b->cols,b->rows);
}
int solve_ILP(int **mat, int b_cols,int b_rows){
	/*0 - no solution
	 * 1 - yes sol*/
	GRBenv *env; GRBmodel *model;
	int dim, res;
	int       *ind;/*[DIM]*/
	double    *val;/*[DIM]*/
	double    *lb;/*[DIM*DIM*DIM]*/
	char      *vtype;/*[DIM*DIM*DIM]*/
	char     **names;/*[DIM*DIM*DIM]*/
	char      *namestorage;/*[10*DIM*DIM*DIM]*/
	char     *cursor;
	int       optimstatus;
	double    objval;

	int       i, j, v, ig, jg, count;
	int       error;

	error = 0; dim = b_cols*b_rows;	env = NULL;	model = NULL;
	ind = (int*)malloc(sizeof(int) * dim);
	exit_malloc_failed(ind == NULL);
	val = (double *)malloc(sizeof(double) * dim);
	exit_malloc_failed(val == NULL);
	lb = (double *)malloc(sizeof(double) * dim * dim * dim);
	exit_malloc_failed(lb == NULL);
	vtype = (char *)malloc(sizeof(char) * dim * dim * dim);
	exit_malloc_failed(vtype == NULL);
	names = (char **)malloc(sizeof(char *) * dim * dim * dim);
	exit_malloc_failed(names == NULL);
	namestorage = (char *)malloc(10 * sizeof(char) * dim * dim * dim);
	exit_malloc_failed(namestorage == NULL);
	cursor = namestorage;
	  for (i = 0; i < dim; i++) {
	    for (j = 0; j < dim; j++) {
	      for (v = 0; v < dim; v++) {
	        if (mat[i][j] == v)
	          lb[i*dim*dim+j*dim+v] = 1;
	        else
	          lb[i*dim*dim+j*dim+v] = 0;
	        vtype[i*dim*dim+j*dim+v] = GRB_BINARY;

	        names[i*dim*dim+j*dim+v] = cursor;
	        sprintf(names[i*dim*dim+j*dim+v], "x[%d,%d,%d]", i, j, v+1);
	        cursor += strlen(names[i*dim*dim+j*dim+v]) + 1;
	      }
	    }
	  }

	  /* Create environment */
	  error = GRBloadenv(&env, "sudoku.log");
	  if (error){
		    printf("ERROR: %s\n", GRBgeterrormsg(env));
		    exit(1);
	  }
	  /* Create new model */
	  error = GRBnewmodel(env, &model, "sudoku", dim*dim*dim, NULL, lb, NULL, vtype, names);
	  if (error){
		    printf("ERROR: %s\n", GRBgeterrormsg(env));
		    exit(1);
	  }
	  /* Each cell gets a value */

	  for (i = 0; i < dim; i++) {
	    for (j = 0; j < dim; j++) {
	      for (v = 0; v < dim; v++) {
	        ind[v] = i*dim*dim + j*dim + v;
	        val[v] = 1.0;
	      }
	      error = GRBaddconstr(model, dim, ind, val, GRB_EQUAL, 1.0, NULL);
		  if (error){
			    printf("ERROR: %s\n", GRBgeterrormsg(env));
			    exit(1);
		  }
	    }
	  }
	  /* Each value must appear once in each row */
	  for (v = 0; v < dim; v++) {
	    for (j = 0; j < dim; j++) {
	      for (i = 0; i < dim; i++) {
	        ind[i] = i*dim*dim + j*dim + v;
	        val[i] = 1.0;
	      }
	      error = GRBaddconstr(model, dim, ind, val, GRB_EQUAL, 1.0, NULL);
		  if (error){
			    printf("ERROR: %s\n", GRBgeterrormsg(env));
			    exit(1);
		  }
	    }
	  }
	  /* Each value must appear once in each column */
	  for (v = 0; v < dim; v++) {
	    for (i = 0; i < dim; i++) {
	      for (j = 0; j < dim; j++) {
	        ind[j] = i*dim*dim + j*dim + v;
	        val[j] = 1.0;
	      }

	      error = GRBaddconstr(model, dim, ind, val, GRB_EQUAL, 1.0, NULL);
		  if (error){
			    printf("ERROR: %s\n", GRBgeterrormsg(env));
			    exit(1);
		  }
	    }
	  }
	  /* Each value must appear once in each subgrid */
	  for (v = 0; v < dim; v++) {
	    for (ig = 0; ig < b_cols; ig++) {
	      for (jg = 0; jg < b_rows; jg++) {
	        count = 0;
	        for (i = ig*b_rows; i < (ig+1)*b_rows; i++) {
	          for (j = jg*b_cols; j < (jg+1)*b_cols; j++) {
	            ind[count] = i*dim*dim + j*dim + v;
	            val[count] = 1.0;
	            count++;
	          }
	        }

	        error = GRBaddconstr(model, dim, ind, val, GRB_EQUAL, 1.0, NULL);
		  if (error){
				    printf("ERROR: %s\n", GRBgeterrormsg(env));
				    exit(1);
			  }
	      }
	    }
	  }
	  /* Optimize model */
	  error = GRBoptimize(model);
	  if (error){
			    printf("ERROR: %s\n", GRBgeterrormsg(env));
			    exit(1);
		  }
	  /* Capture solution information */
	  error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	  if (error){
			    printf("ERROR: %s\n", GRBgeterrormsg(env));
			    exit(1);
		  }
	  error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
	  if (error){
			    printf("ERROR: %s\n", GRBgeterrormsg(env));
			    exit(1);
		  }
	  res = optimstatus == GRB_OPTIMAL;
	/* Free model */
	GRBfreemodel(model);
	/* Free environment */
	GRBfreeenv(env);
	return res;
}

int count_sols(BOARD *b){
	int counter, cell, tmp;
	stack s;
	counter = 0;
	initialize(&s);

	cell = first_empty_cell(b, 0);
	push(cell, &s);
	while (!is_empty_stack(&s)){
		cell = pop(&s);
		tmp = set_next_val(cell, b);
		if (tmp != 0){
			if (!boardSolved(b)){
				push(cell, &s);
				cell = first_empty_cell(b, cell);
			}else{
				counter++;
				set_next_val(cell, b);
			}
		}
	}
	return counter;
}

int set_next_val(int cell, BOARD *b){
	int i, cell_col, cell_row;
	int b_cols, b_rows, side;

	b_cols = b->cols;
	b_rows = b->rows;
	side = (b_cols * b_rows);

	cell_col = cell % side;
	cell_row = cell / side;
	for (i = (b->gameBoard[cell_col][cell_row])+1; i <= side; i++){
		b->gameBoard[cell_col][cell_row] = i;
		if (!isErrCell(cell_col, cell_row, b)){
			return i;
		}
	}
	b->gameBoard[cell_col][cell_row] = 0;
	return 0;
}
