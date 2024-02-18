#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "7ed.h"

#define USAGE ""
#define PROGRAM_NAME "7ed"

int main (int argc, char *argv[]) {


int opt;
int returnval;
long focus = 100;
char *line;
while ((opt = getopt(argc, argv, "i:")) != -1) {
        switch (opt) {
        
	    case 'i':

            returnval = GET_LINE(optarg, focus, &line);
            if (returnval == 1) {
                return EXIT_FAILURE;
            }
            printf("%s", line);
            free(line);
            
	    break;

        default: 
            fprintf(stderr, "%s", USAGE);
            return EXIT_FAILURE;
        }
}
    
    if (argc == 1) {
        fprintf(stderr, "%s: Please provide a file.\n%s", argv[0], USAGE);
	    return EXIT_FAILURE;
    }

return EXIT_SUCCESS;
// TEST!
// LINE 44 YOOOOOOOOOOOOOOO!!
