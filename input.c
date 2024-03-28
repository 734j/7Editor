#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "7ed.h"
#include "input.h"
#include "i_validation.h"
#include <stdint.h>

int smode_input(char *single, char **multiple, uint64_t focus) { // This function is for input then calls the appropriate validator 

    // char *single is for p, e, c, q, a
    // char **multiple is for L, n, x and d. Although it can be expanded to be used in p and e. 

    char smode_buf[SMODE_MAX_SIZE] = { '\0' }; // Smode buffer
    fprintf(stdout, "(%lu): ", focus); // UI
    fgets(smode_buf, SMODE_MAX_SIZE, stdin); // Read user input
    
    int sbl = 0;
    for ( ; sbl < SMODE_MAX_SIZE ; sbl++) {
        if(smode_buf[sbl] == '\0') {
            break;
        }
    }
    if (sbl >= SMODE_MAX_INPUT_SIZE) {
        fprintf(stderr, "sbl > SMODE_MAX_SIZE\n");
        return _FAIL;
    }

    switch (smode_buf[0]) {
                // from L to D there will be the 'Multiple' options. In their respective cases i will check if the input is valid or not.
                // I will not make the program clean the input because that could lead to assumptions.
        case 'l':
        case 'L': {
            
            int chk = validate_L(smode_buf);
            if (chk == _INVALID) {
                return _FAIL;
            }
            *multiple = (char *)malloc(strlen(smode_buf) + 1);
            strcpy(*multiple, smode_buf);
            return _MULTIPLE;

        break; }
        case 'n':
        case 'N': {
            
            int chk = validate_N(smode_buf);
            if (chk == _INVALID) {
                return _FAIL;
            }
            *multiple = (char *)malloc(strlen(smode_buf) + 1);
            strcpy(*multiple, smode_buf);
            return _MULTIPLE;
        break; }
        case 'x':
        case 'X':
            return _MULTIPLE;
        break;
        case 'd':
        case 'D':

            *multiple = (char *)malloc(strlen(smode_buf) + 1); // just a test
            strcpy(*multiple, smode_buf);
            return _MULTIPLE;
        break; // singles below this point
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

    }

    return _FAIL;

}

int main () {

    // running smode_input will do a few things:
    /*
    scenario 1: If you input a valid multiple then the function return multiple
    and it also returns the valid string to the char pointer you provide to the function

    scenario 2: If you input an invalid string, then it will return _FAIL which is basically just '?'
    nothing will be returned to multiple nor single

    scenario 3: if you input a valid single then it will return _SINGLE
    and it will also return the valid single character to the char you provide to the function

    scenario 4: if you input an invalid single then it will return _FAIL which is basically just '?'
    nothing will be returned to multiple nor single
    this scenario is the same as scenario 3. 
    */
    
    char *multiple;
    char single;
    uint64_t focus = 1;
    int si_ret = smode_input(&single, &multiple, focus);
    
    if (si_ret == _SINGLE) {
        fprintf(stdout, "single\n");
        fprintf(stdout, "%c", single);
    }
    if (si_ret == _MULTIPLE) {
        fprintf(stdout, "multiple\n");
        fprintf(stdout, "%s", multiple);
        free(multiple);
    }
    if (si_ret == _FAIL) {
        fprintf(stdout, "?\n");
    }


}