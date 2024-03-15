#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "7ed.h"
#include "input.h"
#include <stdint.h>

// Hej hej!!

#define VERSION 1
#define USAGE "7ed [OPTION] [FILENAME]\n" \
              "\nVALID COMMAND-LINE OPTIONS:\n\n"\
              "    -v   Prints out the version number\n"\
              "    -i   Input file\n\n"\
              "To use the text editor you need to enter appropriate commands.\n"\
              "The prompt will display the current 'Focus' which is the line number\n"\
              "that you are currently focused on. So for example the prompt can look like this:\n"\
              "\n"\
              "(1): \n"\
              " \n"\
              "The '1' is the line number that you are on and after the ':' is where you can\n"\
              "enter commands.\n"\
              "\n"\
              "VALID COMMANDS:\n\n"\
              "    L   Opens a new prompt asking for the line number you want to change 'focus' to\n"\
              "    C   Prints out the filename and the amount of lines in the file to stdout\n"\
              "    P   Prints out the current line that is in 'focus'\n"\
              "    A   Prints out the entire file contents to stdout\n"\
              "    Q   Quit the program\n"\
              "    E   Enter 'edit mode' on the current line in 'focus'\n"\
              "    N   Create a newline on the line after focus\n"\
              "    X   Remove all contents of a line except the newline itself\n"\
              "    D   Remove all contents of a line including the newline itself\n"\


#define PROGRAM_NAME "7ed"

int main (int argc, char *argv[]) {

    if (argc == 1) {
        fprintf(stderr, "%s: Please provide a file.\n%s\n", argv[0], USAGE);
        return EXIT_FAILURE;
    }

    int i_used = FALSE_7ED; // These flags are to make sure -v and -i dont get used together. It makes the program feel a bit nicer.
    int v_used = FALSE_7ED;

    int opt;
    int returnval;
    char *optarg_copy = NULL; // We will copy optarg to this
    while ((opt = getopt(argc, argv, "i:v")) != -1) {
        switch (opt) {
        case 'i':

            optarg_copy = malloc(strlen(optarg) + 1); // malloc memory for optarg_copy
            strcpy(optarg_copy, optarg); // Copy optarg to optarg_copy
            i_used = TRUE_7ED;

        break;
        case 'v':

            v_used = TRUE_7ED;
        break;

        }
    }
    if (i_used == TRUE_7ED && v_used == TRUE_7ED) { // If both flags are used print usage and exit
        free(optarg_copy);
        fprintf(stderr, "%s", USAGE);
        return EXIT_FAILURE;
    }
    
    if (i_used == TRUE_7ED) { // enter startmode if we used 'i'
        returnval = startmode(optarg_copy);
        if (returnval == 1) {
            free(optarg_copy);
            return EXIT_FAILURE;
        }
        free(optarg_copy);
    }
    if (v_used == TRUE_7ED) { // print version
        fprintf(stdout, "7Editor Version %d\n", VERSION);
    }

    return EXIT_SUCCESS;

}
