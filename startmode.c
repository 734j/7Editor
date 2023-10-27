#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "7ed.h"

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
                        printf("Sorry, this number is too small or too large.\n");
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
                    fprintf(stdout, "L is either less than 1 or more than %ld\n", Flines);
                } else {
                    focus = Lfocus;
                }

            break;
            case 'P':
            case 'p':
                
                char *line;
                int ret = GET_LINE(filename, focus, &line);
                if (ret == 1) {
                    return EXIT_FAILURE;
                }
                printf("%s", line);
                free(line);

            break;
            case 'E':
            case 'e':
                printf("EDIT MODE\n");
            break;
            case 'C':
            case 'c':
                
                size_t Flines;
                int returnval = COUNT_LINES_IN_FILE(filename, &Flines);
                if (returnval == 1) {
                    return EXIT_FAILURE;
                }
                fprintf(stdout,"%s %ld lines\n", filename, Flines);

            break;
            case 'Q':
            case 'q':
                return EXIT_SUCCESS;
            break;
            default:
                printf("?\n");
        }



    }
    
    return EXIT_SUCCESS;
}