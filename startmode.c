#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "7ed.h"
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

int startmode(char filename[]) {
    // The entry to the program. Count lines and display the count. Also show which file is being edited.
    uint64_t Flines;
    int returnval = count_lines_in_file(filename, &Flines); 
    if (returnval == 1) {
        return EXIT_FAILURE;
    }
    fprintf(stdout,"%s %lu lines\n", filename, Flines);

    uint64_t focus = 1; // The focus variable. Which is the actual line number we have "selected"
    while(1) { // The main loop to get the "UI" started
        firstwhile:

        int ret = count_lines_in_file(filename, &Flines); 
        if (ret == 1) {
            return EXIT_FAILURE;
        }                                    

        fprintf(stdout, "(%lu): ", focus);
        char command = getchar();
        if (command == '\n') { 
            continue; 
        }
        while ('\n' != getchar());
        
        switch (command) { 
            case 'L':
            case 'l': // L is how we change out "focus"
                uint64_t Lfocus = 0;
                char buf[1024];
                int success;

                do {
                    fprintf(stdout, "(L): ");
                    if (!fgets(buf, 1024, stdin)) {
                        fprintf(stderr, "Too many characters\n");
                        break;
                    }
                    if (buf[0] == '\n') {
                        goto firstwhile; // If the user presses enter then we just goto start of the main loop
                    }
                    if (buf[0] == '+') { // if user inputs + then increment by 1
                        Lfocus = focus+1;
                        break;
                    }
                    if (buf[0] == '-') { // If user inputs - then decrement by 1
                        Lfocus = focus-1;
                        break;
                    }

                    char *endptr;

                    Lfocus = strtol(buf, &endptr, 10);
                    errno = 0;
                    if (errno == ERANGE) {
                        fprintf(stderr, "Sorry, this number is too small or too large.\n");
                        success = 0;
                    }
                    else if (endptr == buf) {
                        // no character was read
                        success = 0;
                    }
                    else if (*endptr && *endptr != '\n') {
                        // *endptr is neither end of string nor newline,
                        // so we didn't convert the *whole* input
                        success = 0;
                    }

                    else {
                        success = 1;
                    }

                } while (!success);

                if (Lfocus < 1 || Lfocus > Flines) {
                    fprintf(stderr, "L is either less than 1 or more than %lu\n", Flines);
                } else {
                    focus = Lfocus;
                }

            break;
            case 'P':
            case 'p':
                
                char *line;
                size_t start;
                int ret = get_line(filename, focus, &line, &start);
                if (ret == 1) {
                    return EXIT_FAILURE;
                }
                fprintf(stdout, "%s", line);
                free(line);

            break;
            case 'E':
            case 'e':
                
                editmode(filename, focus);

            break;
            case 'C':
            case 'c':
                
                uint64_t CFlines;
                int returnval = count_lines_in_file(filename, &CFlines);
                if (returnval == 1) {
                    return EXIT_FAILURE;
                }
                fprintf(stdout,"%s %zu lines\n", filename, CFlines);

            break;
            case 'Q':
            case 'q':
                return EXIT_SUCCESS;
            break;
            case 'a':
            case 'A':
                ncat(filename);
            break;
            case 'n':
            case 'N': {
                 // The "focus" that the newline will be inserted afterwards
                uint64_t new_line_pos_temp = 0; // temp
                char buf[1024];
                int success;

                do {
                    fprintf(stdout, "Create a new line after: ");
                    if (!fgets(buf, 1024, stdin)) {    // take input from user
                        fprintf(stderr, "Too many characters\n");
                        break;
                    }
                    char *endptr;

                    new_line_pos_temp = strtol(buf, &endptr, 10);
                    errno = 0;
                    if (errno == ERANGE) {
                        fprintf(stderr, "Sorry, this number is too small or too large.\n");
                        success = 0;
                    }
                    else if (endptr == buf) {
                        // no character was read
                        success = 0;
                    }
                    else if (*endptr && *endptr != '\n') {
                        success = 0;
                    }

                    else {
                        success = 1;
                    }

                    } while (!success);
                    new_line(filename, new_line_pos_temp);
            break; }
            case 'X':
            case 'x':
                
                int choic = choice();
                if (choic == 0) {
                    remove_line_contents(filename, focus);
                } 
            break;
            case 'D':
            case 'd': {
                int choic = choice();
                if (choic == 1) {
                    return 1;
                }
                int increment = 0;
                if (focus == 1) { // checks if its line 1. This is so that we can remove the newline properly.
                    increment++;
                }
                int rlc = remove_line_contents(filename, focus);
                if (rlc == 1) {
                    return 1;
                }

                int dsn = delete_specified_newline(filename, focus+increment);
                if (dsn == 1) {
                    return 1;
                }
                if (focus == 1) {
                    break;
                }
                focus--;

            break; }
            default:
                fprintf(stdout, "?\n");
        
        }


    
    }

    
    return EXIT_SUCCESS;
}
