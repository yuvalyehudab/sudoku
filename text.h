/*
 * text.h
 *
 *  Created on: 8 בספט׳ 2018
 *      Author: yuval_000
 */

#ifndef SRC_TEXT_H_
#define SRC_TEXT_H_

#define COMMANDS_DICT "exit solve edit print_board set validate undo redo save num_solutions reset mark_errors autofill hint generate"
#define MSG_INVALID_COMMAND "ERROR: invalid command\n"
#define GREETING "Sudoku\n------\n"
#define MSG_MALLOC_FAIL "Error: malloc failed, exiting."
#define C '|'
#define D "%2d"
#define E '.'
#define F '*'
#define MSG_ENTER_CMD "Enter your command:\n"
#define MSG_FILE_ERR_SOLVE "Error: File doesn't exist or cannot be opened\n"
#define MSG_FILE_ERR_EDIT "Error: File cannot be opened\n"
#define MSG_MARK_INVALID_PARAM "Error: the value should be 0 or 1\n"

#define MSG_VAL_RANGE "Error: value not in range %d-%d\n"
#define HINT_MIN 1
#define SET_MIN 0

#define MSG_CELL_FIXED "Error: cell is fixed\n"
#define MSG_PUZZLE_ERR "Puzzle solution is erroneous\n"
#define MSG_PUZZLE_SOLVED "Puzzle solved successfully\n"
#define MSG_ERR_VALS "Error: board contains erroneous values\n"
#define MSG_BOARD_VALID "Validation passed: board is solvable\n"
#define MSG_BOARD_INVALID "Validation failed: board is unsolvable\n"
#define MSG_GEN_RANGE "Error: value not in range 0-%d\n"
#define MSG_NOT_EMPTY "Error: board is not empty\n"

#define MSG_GEN_FAIL "Error: puzzle generator failed\n"
#define MSG_ERR_UNDO "Error: no moves to undo\n"
#define MSG_MOVE_UNDO "Undo %d,%d: from %d to %d\n"
#define MSG_ERR_REDO "Error: no moves to redo\n"
#define MSG_MOVE_REDO "Redo %d,%d: from %d to %d\n"
#define UNDER_SCORE '_'
#define MSG_VAL_FAIL "Error: board validation failed\n"
#define MSG_FILE_SAVE "Error: file cannot be created or modified\n"
#define MSG_SAVED_TO "Saved to: %s\n"

#define MSG_CELL_FULL "Error: cell already contains a value\n"
#define AH "Error: board is unsolvable\n"
#define MSG_HINT "Hint: set cell to %d\n"
#define MSG_SOL_AMT "Number of solutions: %d\n"
#define MSG_GOOD_BOARD "This is a good board!\n"
#define MSG_MORE_SOLS "The puzzle has more than 1 solution, try to edit it further\n"
#define MSG_CELL_CHANGED "Cell <%d,%d> set to %d\n"
#define MSG_RESET "Board reset\n"
#define MSG_EXT "Exiting...\n"

#endif /* SRC_TEXT_H_ */
