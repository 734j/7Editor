#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "7ed.h"
#include "input.h"
#include "i_validation.h"
#include <stdint.h>

int call_L_plus_continue(char *multiple) {

    fprintf(stdout, "+ or - CONTINUE %s\n", multiple);
    return 0;
}

uint64_t call_L_plus_minus_only(uint64_t focus, char p_or_m) {
    switch (p_or_m) {
        case '+':
            return focus+1;
        break;
        case '-':
            return focus-1;
        break;
    }

    return _NA;
}

uint64_t call_L(char *multiple, uint64_t focus, uint64_t Flines) {

    int imm = _IMM_NUMBER;

    if (multiple[1] == '\n') {
        imm = _NA;
        fprintf(stdout, "L only\n");
    }

    if (multiple[1] == '+' || multiple[1] == '-') {
        imm = _NA;
        if (multiple[2] == '\n') { // This is where - and + only inputs happen and get processed
            focus = call_L_plus_minus_only(focus, multiple[1]);
            return focus;
        }
        call_L_plus_continue(multiple);

    }

    if (imm == _IMM_NUMBER) {
        fprintf(stdout, "immediate\n");
    }


    return focus;
}

int call_N(char *multiple) {

    fprintf(stdout, "%s\n", multiple);
    return 0;
}

int call_X(char *multiple) {

    fprintf(stdout, "%s\n", multiple);
    return 0;
}

int call_D(char *multiple) {

    fprintf(stdout, "%s\n", multiple);
    return 0;
}

