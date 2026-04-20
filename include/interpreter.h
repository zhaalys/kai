#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define TAPE_SIZE 30000

typedef struct {
    uint8_t tape[TAPE_SIZE];
    uint32_t data_pointer;
    uint32_t instruction_pointer;
    char *code;
    uint32_t code_size;
    uint32_t *jump_table; // Pre-computed brackets for performance
} BFState;

/**
 * Initializes the Brainfuck state.
 * Returns 0 on success, non-zero on failure.
 */
int bf_init(BFState *state, const char *code);

/**
 * Executes the Brainfuck code in the state.
 * Returns 0 on success, non-zero on failure.
 */
int bf_execute(BFState *state);

/**
 * Cleans up memory allocated for the Brainfuck state.
 */
void bf_cleanup(BFState *state);

#endif // INTERPRETER_H
