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

    }

    return _FAIL;

}
