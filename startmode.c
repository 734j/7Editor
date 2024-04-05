#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "7ed.h"
#include "input.h"
#include "i_validation.h"
#include "process_multiples.h"
#include <stdint.h>

extern int clfstdin_doubleprint;

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

int call_singles(char single, uint64_t focus, char *filename) {

    switch(single) {
        case 'p':
        case 'P':

            char *line;
            size_t start;
            int ret = get_line(filename, focus, &line, &start);
            if (ret == 1) {
                return EXIT_FAILURE;
            }
            fprintf(stdout, "%s", line);
            free(line);

        break;
        case 'e':
        case 'E':

            editmode(filename, focus);
            
        break;
        case 'c':
        case 'C':

            int dnl = display_name_linecount(filename);
            if (dnl == 1) {
                return EXIT_FAILURE;
            }

        break;
        case 'q':
        case 'Q':

            exit(EXIT_SUCCESS);

        break;
        case 'a':
        case 'A':

            ncat(filename);

        break;
    }

    return 0;
}

int startmode(char filename[]) {

    // The entry to the program. Count lines and display the count. Also show which file is being edited.
    uint64_t Flines;
    int dnl = display_name_linecount(filename);
    if (dnl == 1) {
        return EXIT_FAILURE;
    }
    uint64_t focus = 1; // The focus variable. Which is the actual line number we have "selected"

    while (1) {
        char *multiple;
        char single;
        int clif = count_lines_in_file(filename, &Flines);
        if (clif == 1) {
            return EXIT_FAILURE;
        }

        int smode_input_ret = smode_input(&single, &multiple, focus);

        switch (smode_input_ret) {

            case _SINGLE:
                call_singles(single, focus, filename);
            break;
            case _MULTIPLE:
                switch(multiple[0]) {
                    case 'l':
                    case 'L':
                        focus = call_L(multiple, focus, Flines);
                    break;  
                    case 'n':
                    case 'N':
                        call_N(multiple, focus, Flines, filename);
                    break;
                    case 'x':
                    case 'X':
                        call_X(multiple);
                    break;
                    case 'd':
                    case 'D':
                        call_D(multiple);
                    break;
                }

                free(multiple);
            break;
            case _FAIL:

                if (clfstdin_doubleprint == 0) { 
                    clfstdin_doubleprint = 1;
                    break; 
                }
                fprintf(stdout, "?\n");
            break;
            case _RETURN: // if user just preses 'return' button
            break;

        }
        
    }
}
