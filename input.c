#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "7ed.h"
#include "input.h"
#include "i_validation.h"
#include <stdint.h>

int clfstdin_doubleprint = 1; // variable accessed via extern int in startmode so that it does not print '?' 2 times in a row

int check_length_fix_stdin(char *smode_buf) {

    int not_newline = 1;
    switch (smode_buf[0]) { // This whole thing just checks for 'single' inputs. The single inputs can ONLY and should only allow for single characters.
        case 'p':
        case 'P':
        case 'e':
        case 'E':
        case 'c':
        case 'C':
        case 'q':
        case 'Q':
        case 'a':
        case 'A':
            if (smode_buf[1] != '\n') { // If not a new line 
                not_newline = 0;
                break;
            } else {
                return 0;
            }
        break;
    }

    int sbl = 0;
    for ( ; sbl < SMODE_MAX_INPUT_SIZE ; sbl++) {
        if(smode_buf[sbl] == '\0' || smode_buf[sbl] == '\n') {
            break;
        }
    }
    if (sbl >= SMODE_MAX_INPUT_SIZE) {
        char c;
        while ((c = getchar()) != '\n');
        fprintf (stdout, "?\n");
        clfstdin_doubleprint = 0;
        //fprintf (stderr, "sbl > SMODE_MAX_SIZE\n"); // debug code
        return _FAIL;
    }

    if (not_newline == 0) {
        return _FAIL;
    }

    return 0;
}

int smode_input(char *single, char **multiple, uint64_t focus) { // This function is for input then calls the appropriate validator 

    // char *single is for p, e, c, q, a
    // char **multiple is for L, n, x and d. Although it can be expanded to be used in p and e. 

    char smode_buf[SMODE_MAX_SIZE] = { '\0' }; // Smode buffer
    fprintf(stdout, "(%lu): ", focus); // UI
    fgets(smode_buf, SMODE_MAX_SIZE, stdin); // Read user input
    if(check_length_fix_stdin(smode_buf) == _FAIL) { return _FAIL; }

    switch (smode_buf[0]) {
                // from L to D there will be the 'Multiple' options. In their respective cases i will check if the input is valid or not.
                // I will not make the program clean the input because that could lead to assumptions.
        case 'l':
        case 'L': {
            
            int chk = validate_LN(smode_buf, MODE_L);
            if (chk == _INVALID) {
                return _FAIL;
            }
            *multiple = (char *)malloc(strlen(smode_buf) + 1);
            strcpy(*multiple, smode_buf);
            return _MULTIPLE;

        break; }
        case 'n':
        case 'N': {
            
            int chk = validate_LN(smode_buf, MODE_N);
            if (chk == _INVALID) {
                return _FAIL;
            }
            *multiple = (char *)malloc(strlen(smode_buf) + 1);
            strcpy(*multiple, smode_buf);
            return _MULTIPLE;

        break; }
        case 'x':
        case 'X': {

            // X will work with MODE_N mode for now. I may update in the future to use MODE_L but for now i will only use MODE_N for simplicity sake.
            int chk = validate_LN(smode_buf, MODE_N);
            if (chk == _INVALID) {
                return _FAIL;
            }
            *multiple = (char *)malloc(strlen(smode_buf) + 1);
            strcpy(*multiple, smode_buf);
            return _MULTIPLE;

        break; }
        case 'd':
        case 'D': {
            
            // X will work with MODE_N mode for now. I may update in the future to use MODE_L but for now i will only use MODE_N for simplicity sake.
            int chk = validate_LN(smode_buf, MODE_N);
            if (chk == _INVALID) {
                return _FAIL;
            }
            *multiple = (char *)malloc(strlen(smode_buf) + 1);
            strcpy(*multiple, smode_buf);
            return _MULTIPLE;

        break; }// singles below this point
        case 'p':
        case 'P':
            *single = smode_buf[0];
            return _SINGLE;
        break;
        case 'e':
        case 'E':
            *single = smode_buf[0];
            return _SINGLE;
        break;
        case 'c':
        case 'C':
            *single = smode_buf[0];
            return _SINGLE;
        break;
        case 'q':
        case 'Q':
            *single = smode_buf[0];
            return _SINGLE;
        break;
        case 'a':
        case 'A':
            *single = smode_buf[0];
            return _SINGLE;
        break;
        case '\n':
            return _RETURN;
        break;

    }

    return _FAIL;

}
