#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "7ed.h"

int NCAT(char filename[]) {

    FILE *file;
    file = fopen(filename,"r");

    if (file == NULL) {
        fprintf(stderr, "Cannot open file.\n");
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

    size_t Flines;
    int returnval = COUNT_LINES_IN_FILE(filename, &Flines);
    if (returnval == 1) {
        return EXIT_FAILURE;
    }
    fprintf(stdout,"%s %ld lines\n", filename, Flines);

    long focus = 1;

    while(1) {

        fprintf(stdout, "(%ld): ", focus);
        char command = getchar();
        if (command == '\n') { 
            continue; 
        }
        while ('\n' != getchar());
        
        switch (command) {
            case 'L':
            case 'l':
                long Lfocus;
                char buf[1024];
                int success;

                do {
                    fprintf(stdout, "(L): ");
                    if (!fgets(buf, 1024, stdin)) {
                        fprintf(stderr, "Too many characters\n");
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

                if (Lfocus < 1 || Lfocus > (long)Flines) {
                    fprintf(stderr, "L is either less than 1 or more than %ld\n", Flines);
                } else {
                    focus = Lfocus;
                }

            break;
            case 'P':
            case 'p':
                
                char *line;
                size_t start;
                int ret = GET_LINE(filename, focus, &line, &start);
                if (ret == 1) {
                    return EXIT_FAILURE;
                }
                fprintf(stdout, "%s", line);
                //printf("%ld", start);
                free(line);

            break;
            case 'E':
            case 'e':
                
                editmode(filename, focus);

            break;
            case 'C':
            case 'c':
                
                size_t CFlines;
                int returnval = COUNT_LINES_IN_FILE(filename, &CFlines);
                if (returnval == 1) {
                    return EXIT_FAILURE;
                }
                fprintf(stdout,"%s %ld lines\n", filename, CFlines);

            break;
            case 'Q':
            case 'q':
                return EXIT_SUCCESS;
            break;
            case 'a':
            case 'A':
                NCAT(filename);
            break;
            default:
                fprintf(stdout, "?\n");
        }



    }
    
    return EXIT_SUCCESS;
}