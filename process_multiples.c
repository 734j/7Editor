#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "7ed.h"
#include "input.h"
#include "i_validation.h"
#include <stdint.h>

int check_L_linecount(uint64_t Flines, uint64_t focus, int mode) { // Check if you are trying to modify or move to lines under 1 or lines that are over the total amount of lines in the file.
                                                                    // Mode N allows us to go to line 0 because N needs it.
    if (mode == MODE_N) { 
        if (focus == 0) {
            return _VALID;
        }
    }

    if (focus < 1 || focus > Flines) {
        fprintf(stdout, "?\n");
        return _INVALID;
    }
    return _VALID;
}

uint64_t call_L_immediate(char *multiple, uint64_t focus, uint64_t Flines) { // Go to a specific newline

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
    
    if(check_L_linecount(Flines, new_focus, MODE_L) == _INVALID) {
        return focus;
    }

    return new_focus;

}

uint64_t call_L_plus_minus_continue(char *multiple, uint64_t focus, uint64_t Flines) { // Go up or down a number of lines

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
    if(check_L_linecount(Flines, new_focus, MODE_L) == _INVALID) {
        return focus;
    }

    return new_focus;
}

uint64_t call_L_only(uint64_t focus, uint64_t Flines) { // Old L functionality.

    char buf[SMODE_MAX_SIZE] = { '\0' };
    fprintf(stdout, "(L): ");
    fgets(buf, SMODE_MAX_SIZE, stdin);
    if(check_length_fix_stdin(buf) == _FAIL) { return focus; return _FAIL; }

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

    int cll = check_L_linecount(Flines, new_focus, MODE_L);
    if (cll == _INVALID) {
        return focus;
    }

    return new_focus;
}

uint64_t call_L_plus_minus_only(uint64_t focus, char p_or_m, uint64_t Flines) { // Go up or down by only 1
    switch (p_or_m) {
        case '+': {
            int cll = check_L_linecount(Flines, focus+1, MODE_L);
            if (cll == _INVALID) {
                return focus;
            }
            return focus+1;
        break; }
        case '-': {
            int cll = check_L_linecount(Flines, focus-1, MODE_L);
            if (cll == _INVALID) {
                return focus;
            }
            return focus-1;
        break; }
    }

    return _NA;
}

uint64_t call_L(char *multiple, uint64_t focus, uint64_t Flines) { // Main L function to decide what to do.

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
        focus = call_L_immediate(multiple, focus, Flines);
        return focus;
    }


    return focus;
}

int call_N_immediate(char *multiple, uint64_t Flines, char *filename) { 
    
    char new_multiple[32] = { '\0' };

    int i = 0;
    int j = 1;
    for( ; multiple[j] != '\n' ; i++, j++) {
        new_multiple[i] = multiple[j];
    }
    
    char *endptr;
    uint64_t newline_insert_position = strtol(new_multiple, &endptr, 10);
    errno = 0;
    if (errno == ERANGE) {
        return 0;
    }
    if (endptr == new_multiple) {
        return 0;
    }
    
    if(check_L_linecount(Flines, newline_insert_position, MODE_N) == _INVALID) {
        return -1;
    }

    new_line(filename, newline_insert_position);

    return 0;

}

int call_N_plus_continue(char *multiple, uint64_t focus, char *filename) {

    char new_multiple[32] = { '\0' };

    int choice_yesno = 0;
    int i = 0;
    int j = 2;
    for( ; multiple[j] != '\n' ; i++, j++) {
        new_multiple[i] = multiple[j];
    }
    
    char *endptr;
    uint64_t newlines_to_add = strtol(new_multiple, &endptr, 10);
    errno = 0;
    if (errno == ERANGE) {
        return -1;
    }
    if (endptr == new_multiple) {
        return -1;
    }
    
    if (newlines_to_add > 1) {
        choice_yesno = choice();
    }
    if (choice_yesno == 1) { return -1; }
    
    for(uint64_t count = 0 ; count < newlines_to_add ; count++) { // This is very inefficient if you are adding thousands of lines but why would you wanna do that anyway?
        new_line(filename, focus);
    }

    return 0;

}

uint64_t call_N(char *multiple, uint64_t focus, uint64_t Flines, char *filename) {

    int imm = _IMM_NUMBER;

    if (multiple[1] == '\n') { // N will add just 1 line.
        imm = _NA;
        new_line(filename, focus);
        return 0;
    }

    if (multiple[1] == '+') { // N+ is the same as N. It will only add 1 line.
        imm = _NA;
        if (multiple[2] == '\n') {
            new_line(filename, focus);
            return 0;
        }

        call_N_plus_continue(multiple, focus, filename);
        return 0;
    }

    if (imm == _IMM_NUMBER) {
        call_N_immediate(multiple, Flines, filename);
        return 0;
    }

    return 0;
}

int call_X_plus_continue(char *multiple, uint64_t focus, char *filename, uint64_t Flines) {

    char new_multiple[32] = { '\0' };

    int i = 0;
    int j = 2;
    for( ; multiple[j] != '\n' ; i++, j++) {
        new_multiple[i] = multiple[j];
    }
    
    char *endptr;
    uint64_t amount_of_lines_to_remove = strtol(new_multiple, &endptr, 10);
    errno = 0;
    if (errno == ERANGE) {
        return -1;
    }
    if (endptr == new_multiple) {
        return -1;
    }

    if(check_L_linecount(Flines, focus+(amount_of_lines_to_remove-1) /*-1 because we are not doing currentline + amount of lines. That would be 1 too much*/, MODE_L) == _INVALID) {
        return -1; 
    }

    if (choice() == 1) { return -1; }

    for(uint64_t count = 0 ; count < amount_of_lines_to_remove ; count++) { // This is very inefficient
        remove_line_contents(filename, focus+count);
    }

    return 0;
}

int call_X_immediate(char *multiple, uint64_t Flines, char *filename) {

    char new_multiple[32] = { '\0' };

    int i = 0;
    int j = 1;
    for( ; multiple[j] != '\n' ; i++, j++) {
        new_multiple[i] = multiple[j];
    }
    
    char *endptr;
    uint64_t remove_line_contents_position = strtol(new_multiple, &endptr, 10);
    errno = 0;
    if (errno == ERANGE) {
        return 0;
    }
    if (endptr == new_multiple) {
        return 0;
    }
    
    if(check_L_linecount(Flines, remove_line_contents_position, MODE_L) == _INVALID) {
        return -1;
    }

    if (choice() == 1) { return -1; }    

    remove_line_contents(filename, remove_line_contents_position);

    return 0;
}

int call_X(char *multiple, uint64_t focus, uint64_t Flines, char *filename) {
    
    int imm = _IMM_NUMBER;

    if (multiple[1] == '\n') { // X Will remove current line
        imm = _NA;
    
        if(choice() == 0) {
            remove_line_contents(filename, focus);
        }
        return 0;
    }

    if (multiple[1] == '+') {
        imm = _NA;
        if (multiple[2] == '\n') {
            if(choice() == 0) {
                remove_line_contents(filename, focus);
            }
            return 0;
        }
        call_X_plus_continue(multiple, focus, filename, Flines);

        return 0;
    }

    if (imm == _IMM_NUMBER) {
        call_X_immediate(multiple, Flines, filename);
        return 0;
    }

    return 0;
}

uint64_t call_D_plus_continue(char *multiple, uint64_t focus, uint64_t Flines, char *filename) {

    char new_multiple[32] = { '\0' };

    int i = 0;
    int j = 2;
    for( ; multiple[j] != '\n' ; i++, j++) {
        new_multiple[i] = multiple[j];
    }
    
    char *endptr;
    uint64_t lines_and_newlines_to_remove = strtol(new_multiple, &endptr, 10);
    errno = 0;
    if (errno == ERANGE) {
        return focus;
    }
    if (endptr == new_multiple) {
        return focus;
    }

    if(check_L_linecount(Flines, focus+(lines_and_newlines_to_remove-1) /*-1 because we are not doing currentline + amount of lines. That would be 1 too much*/, MODE_L) == _INVALID) {
        return focus; 
    }

    if (choice() == 1) { return focus; }

    for(uint64_t count = 0 ; count < lines_and_newlines_to_remove ; count++) { // This is very inefficient
        remove_line_contents_and_newline(filename, focus);
    }

    return focus-1;

}

uint64_t call_D_immediate(char *multiple, uint64_t focus, uint64_t Flines, char *filename) {
    
    char new_multiple[32] = { '\0' };

    int i = 0;
    int j = 1;
    for( ; multiple[j] != '\n' ; i++, j++) {
        new_multiple[i] = multiple[j];
    }
    
    char *endptr;
    uint64_t remove_line_and_newline_position = strtol(new_multiple, &endptr, 10);
    errno = 0;
    if (errno == ERANGE) {
        return focus;
    }
    if (endptr == new_multiple) {
        return focus;
    }
    
    if(check_L_linecount(Flines, remove_line_and_newline_position, MODE_L) == _INVALID) {
        return focus;
    }

    if (choice() == 1) { return focus; }    

    remove_line_contents_and_newline(filename, remove_line_and_newline_position);
    if (focus == 1 && remove_line_and_newline_position == 1) {
        return 1;
    }

    if (focus == remove_line_and_newline_position) {
        return focus-1;
    }

    return focus;
}

uint64_t call_D(char *multiple, uint64_t focus, uint64_t Flines, char *filename) {

    int imm = _IMM_NUMBER;
    uint64_t original_focus = focus;

    if (multiple[1] == '\n') { // D Will remove current line
        imm = _NA;
    
        if(choice() == 0) {
            remove_line_contents_and_newline(filename, focus);
            if (focus == 1) { return 1; }
            return focus-1;
        } else {
            return focus;
        }
        
    }

    if (multiple[1] == '+') {
        imm = _NA;
        if (multiple[2] == '\n') {
            if(choice() == 0) {
                remove_line_contents_and_newline(filename, focus);
                if (focus == 1) { return 1; }
                return focus-1;
            } else {
                return focus;
            }
        }
        focus = call_D_plus_continue(multiple, focus, Flines, filename);
        if (original_focus == 1) { return 1; }
        return focus;
    }

    if (imm == _IMM_NUMBER) {
        uint64_t calldimmediate; 
        calldimmediate = call_D_immediate(multiple, focus, Flines, filename);
        return calldimmediate;
    }

    return 0;
}

