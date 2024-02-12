#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "7ed.h"
#include <stdint.h>

// This will be the new input system for combining commands with line numbers
// Work in progress and far from finished. This is not included when compiling normally.

#define SMODE_MAX_SIZE 64

int smode_input(char *single, char **multiple, uint64_t focus) {

    // char *single is for p, e, c, q, a
    // char **multiple is for L, n, x and d. Although it can be expanded to be used in p and e. 

    char smode_buf[SMODE_MAX_SIZE]; // Smode buffer
    fprintf(stdout, "(%lu): ", focus); // UI
    fgets(smode_buf, SMODE_MAX_SIZE, stdin); // Read user input
    
    switch (smode_buf[0]) {
                // from L to D there will be the 'Multiple' options. In their respective cases i will check if the input is valid or not.
                // I will not make the program clean the input because that could lead to assumptions.
        case 'l':
        case 'L':
            /*
            Check for + (or -)
            path 1: If there is a + then check if there is anything else after it. If not then return L+.
            1.1: If there is a anything that is not a number from 0-9 then return '?' (safety check)
            1.2: If there is a number after the + then check for another number until there is no more numbers. For example we return L+34
            1.3: If there happens to be something that is not a number from 0-9 in 1.2 case then immediately return '?' (safety check)
            
            path 2: If there is a number from 1-9 immediately after the L then keep checking for numbers (between 0-9) until there is no more.
            A valid return would be something like L24
            2.1: If there is anything that is not a number from 1-9 then return '?'

            path 3: If there is nothing else after the L (in this case just an L) then just return an L
            */
            if (smode_buf[1] == '+') {
                // Check if its a plus
            }

            int num_flag = -1;
            char nums[] = "123456789";
            for (int i = 1 ; i < 9 ; i++) {
                if (smode_buf[1] == nums[i]) {
                    num_flag = 0;
                    break;
                }
            }

            int L_num_flag = NULL;
            if (num_flag == 0) {
                L_num_flag = 0; // True
                char nums_with_zero[] = "0123456789";
                // Check the rest with a loop. Start at 2
                for (int i = 2 ; i < SMODE_MAX_SIZE ; i++) {
                                                     // Nested loop to check every element in nums_with_zero on the current smode element (i)(outer loop)
                    for (int j = 0 ; j < 9 ; j++) {

                        if(smode_buf[i] != nums_with_zero[j]) {//check if its not a number. It could be the null terminator so we check if it is
                            if (smode_buf[i] == '\0') {
                                // success! return the "valid" string. (example: L44)
                                // Do something to indicate that its successful and break out somehow not sure yet
                                break;
                            }
                            L_num_flag = -1; // False. 
                        }

                    } //inner nested loop
                    break; // Is this where we break again to get out of the outer loop?

                }//outer nested loop
            }// num_flag if-statement
            if (L_num_flag == 0) {
                // actually return the valid string here
            } 
            if (L_num_flag == -1) {
                // invalid! return '?'
            }


            if (smode_buf[1] == '\0') {
                // just return L
            }

            // if it gets to here then i guess we just return '?'
            

        break;
        case 'n':
        case 'N':

        break;
        case 'x':
        case 'X':

        break;
        case 'd':
        case 'D':

            *multiple = (char *)malloc(strlen(smode_buf) + 1); // just a test
            strcpy(*multiple, smode_buf);
        break; // singles below this point
        case 'p':
        case 'P':
            *single = smode_buf[0];
        break;
        case 'e':
        case 'E':
            *single = smode_buf[0];
        break;
        case 'c':
        case 'C':
            *single = smode_buf[0];
        break;
        case 'q':
        case 'Q':
            *single = smode_buf[0];
        break;
        case 'a':
        case 'A':
            *single = smode_buf[0];
        break;

    }
    
    //printf("%s\n", smode_buf);
    return 0;

}

int main () {
    char *multiple;
    char single;
    uint64_t focus = 1;
    smode_input(&single, &multiple, focus);
    fprintf(stdout, "%s\n", multiple);
    free(multiple);

}