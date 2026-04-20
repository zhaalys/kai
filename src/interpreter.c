#include "../include/interpreter.h"
#include <string.h>

int bf_init(BFState *state, const char *code) {
    state->code_size = strlen(code);
    state->code = malloc(state->code_size + 1);
    if (state->code) {
        strcpy(state->code, code);
    }
    state->data_pointer = 0;
    state->instruction_pointer = 0;
    memset(state->tape, 0, TAPE_SIZE);

    state->jump_table = calloc(state->code_size, sizeof(uint32_t));
    if (!state->jump_table) return 1;

    // Pre-compute jump table (bracket matching)
    uint32_t *stack = malloc(state->code_size * sizeof(uint32_t));
    uint32_t stack_ptr = 0;

    for (uint32_t i = 0; i < state->code_size; i++) {
        if (state->code[i] == '[') {
            stack[stack_ptr++] = i;
        } else if (state->code[i] == ']') {
            if (stack_ptr == 0) {
                free(stack);
                return 2; // Unbalanced brackets
            }
            uint32_t open_index = stack[--stack_ptr];
            state->jump_table[open_index] = i;
            state->jump_table[i] = open_index;
        }
    }

    free(stack);
    if (stack_ptr != 0) return 3; // Unbalanced brackets

    return 0;
}

int bf_execute(BFState *state) {
    while (state->instruction_pointer < state->code_size) {
        char instruction = state->code[state->instruction_pointer];

        switch (instruction) {
            case '>':
                if (state->data_pointer >= TAPE_SIZE - 1) {
                    fprintf(stderr, "\nError: Tape pointer out of bounds (Right)\n");
                    return 1;
                }
                state->data_pointer++;
                break;
            case '<':
                if (state->data_pointer == 0) {
                    fprintf(stderr, "\nError: Tape pointer out of bounds (Left)\n");
                    return 1;
                }
                state->data_pointer--;
                break;
            case '+':
                state->tape[state->data_pointer]++;
                break;
            case '-':
                state->tape[state->data_pointer]--;
                break;
            case '.':
                putchar(state->tape[state->data_pointer]);
                break;
            case ',':
                state->tape[state->data_pointer] = getchar();
                break;
            case '[':
                if (state->tape[state->data_pointer] == 0) {
                    state->instruction_pointer = state->jump_table[state->instruction_pointer];
                }
                break;
            case ']':
                if (state->tape[state->data_pointer] != 0) {
                    state->instruction_pointer = state->jump_table[state->instruction_pointer];
                }
                break;
        }
        state->instruction_pointer++;
    }
    return 0;
}

void bf_cleanup(BFState *state) {
    if (state->code) free(state->code);
    if (state->jump_table) free(state->jump_table);
}
