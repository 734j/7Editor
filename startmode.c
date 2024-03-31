#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "7ed.h"
#include "input.h"
#include "i_validation.h"
#include <stdint.h>

int ncat(char filename[]) {

    FILE *file;
    file = fopen(filename,"r");

    if (file == NULL) {
        fprintf(stderr, "Cannot open file. ncat\n");
        return 1;
    }

    char buffer[BUF_SZ_2];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BUF_SZ_2, file)) > 0) {
        fwrite(buffer, 1, bytes_read, stdout);
    }
    fclose(file);
    return 0;
}

int display_name_linecount(char *filename) {

    uint64_t Flines;
    int returnval = count_lines_in_file(filename, &Flines); 
    if (returnval == 1) {
        return 1;
    }
    fprintf(stdout,"%s %ld lines\n", filename, Flines);

    return 0;
}

int startmode(char filename[]) {
    // The entry to the program. Count lines and display the count. Also show which file is being edited.
    int dnl = display_name_linecount(filename);
    if (dnl == 1) {
        return EXIT_FAILURE;
    }
    uint64_t focus = 1; // The focus variable. Which is the actual line number we have "selected"

    char *multiple;
    char single;
    int smode_input_ret = smode_input(&single, &multiple, focus);
    if (smode_input_ret == _SINGLE) {
        fprintf(stdout, "single\n");
        fprintf(stdout, "%c\n", single);
    }
    if (smode_input_ret == _MULTIPLE) {
        fprintf(stdout, "multiple\n");
        fprintf(stdout, "%s", multiple);
        free(multiple);
    }
    if (smode_input_ret == _FAIL) {
        fprintf(stdout, "?\n");
    }
    
    return EXIT_SUCCESS;
}
