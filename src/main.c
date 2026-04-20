#include "../include/interpreter.h"
#include <string.h>

/**
 * Loads a file's content into a buffer.
 * Memory for the buffer is allocated and must be freed by the caller.
 */
char* load_file(const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "Error: Could not open input file '%s'\n", filename);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    if (size == 0) {
        fprintf(stderr, "Error: Input file '%s' is empty\n", filename);
        fclose(f);
        return NULL;
    }
    fseek(f, 0, SEEK_SET);

    char* buffer = malloc(size + 1);
    if (!buffer) {
        fprintf(stderr, "Error: Could not allocate memory for input file\n");
        fclose(f);
        return NULL;
    }

    size_t read_bytes = fread(buffer, 1, size, f);
    buffer[read_bytes] = '\0';

    fclose(f);
    return buffer;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename.bf>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    char* source = load_file(filename);

    if (!source) {
        return 1;
    }

    BFState state;
    if (bf_init(&state, source) != 0) {
        fprintf(stderr, "Initialization failed.\n");
        free(source);
        return 1;
    }

    if (bf_execute(&state) != 0) {
        fprintf(stderr, "\nExecution error occurred.\n");
    }

    bf_cleanup(&state);
    free(source);

    return 0;
}
