#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "7ed.h"
#include "input.h"
#include "i_validation.h"
#include <stdint.h>

int check_L_linecount(uint64_t Flines, uint64_t focus) {

    if (focus < 1 || focus > Flines) {
        fprintf(stderr, "check_L_linecount()\n");
        return _INVALID;
    }
    return _VALID;
}

uint64_t call_L_immediate(char *multiple, uint64_t focus, uint64_t Flines) {

    char new_multiple[32] = { '\0' };

    int i = 0;
    int j = 1;
    for( ; multiple[j] != '\n' ; i++, j++) {
        new_multiple[i] = multiple[j];
    }
    
    char *endptr;
    uint64_t new_focus = strtol(new_multiple, &endptr, 10);
    errno = 0;
    if (errno == ERANGE) {
        return focus;
    }
    if (endptr == new_multiple) {
        return focus;
    }
    
    if(check_L_linecount(Flines, new_focus) == _INVALID) {
        return focus;
    }

    return new_focus;

}

uint64_t call_L_plus_minus_continue(char *multiple, uint64_t focus, uint64_t Flines) {

    char new_multiple[32] = { '\0' };

    int i = 0;
    int j = 2;
    for( ; multiple[j] != '\n' ; i++, j++) {
        new_multiple[i] = multiple[j];
    }
    
    char *endptr;
    uint64_t new_focus = strtol(new_multiple, &endptr, 10);
    errno = 0;
    if (errno == ERANGE) {
        return focus;
    }
    if (endptr == new_multiple) {
        return focus;
    }
    
    if(multiple[1] == '+') { new_focus = focus+new_focus; }
    if(multiple[1] == '-') { 
        int64_t s_new_focus = (int64_t)new_focus;
        int64_t s_focus = (int64_t)focus;
        if((s_focus-s_new_focus) <= 0) {
            new_focus = 0;
        } else {
            new_focus = focus-new_focus;
        }
    }
    if(check_L_linecount(Flines, new_focus) == _INVALID) {
        return focus;
    }

    return new_focus;
}

uint64_t call_L_only(uint64_t focus, uint64_t Flines) {

    char buf[33] = { '\0' };
    fprintf(stdout, "(L): ");
    fgets(buf, 32, stdin);
    if(check_length_fix_stdin(buf) == _FAIL) { return _FAIL; }

    if (buf[0] == '\n') {
        return focus;
    }

    char *endptr;
    uint64_t new_focus = strtol(buf, &endptr, 10);
    errno = 0;
    if (errno == ERANGE) {
        return focus;
    }
    if (endptr == buf) {
        return focus;
    }
    if (*endptr != '\n') {
        return focus;
    }

    int cll = check_L_linecount(Flines, new_focus);
    if (cll == _INVALID) {
        return focus;
    }

    return new_focus;
}

uint64_t call_L_plus_minus_only(uint64_t focus, char p_or_m, uint64_t Flines) {
    switch (p_or_m) {
        case '+': {
            int cll = check_L_linecount(Flines, focus+1);
            if (cll == _INVALID) {
                return focus;
            }
            return focus+1;
        break; }
        case '-': {
            int cll = check_L_linecount(Flines, focus-1);
            if (cll == _INVALID) {
                return focus;
            }
            return focus-1;
        break; }
    }

    return _NA;
}

uint64_t call_L(char *multiple, uint64_t focus, uint64_t Flines) {

    int imm = _IMM_NUMBER;

    if (multiple[1] == '\n') {
        imm = _NA;
        focus = call_L_only(focus, Flines);
        return focus;
    }

    if (multiple[1] == '+' || multiple[1] == '-') {
        imm = _NA;
        if (multiple[2] == '\n') { // This is where - and + only inputs happen and get processed
            focus = call_L_plus_minus_only(focus, multiple[1], Flines);
            return focus;
        }
        focus = call_L_plus_minus_continue(multiple, focus, Flines);
        return focus;

    }

    if (imm == _IMM_NUMBER) {
        fprintf(stdout, "immediate\n");
        focus = call_L_immediate(multiple, focus, Flines);
        return focus;
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

