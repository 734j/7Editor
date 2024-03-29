#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "7ed.h"
#include "i_validation.h"
#include "input.h"
#include <stdint.h>

// This will be the new input system for combining commands with line numbers
// Work in progress and far from finished. This is not included when compiling normally.

int validate_check_p_m(char *smode_buf) { // Check for +, -, +0, -0, +\n, or -\n
/*
_INVALID (immediately stop validating and return invalid to everything)
_PLUS_ONLY (L+ only. Immediately valid. Return)
_PLUS_CONTINUE (L+ And more numbers after it. Will make validate_plus_continue run)
_NA (None of these cases)
*/
    char nums[] = "123456789";
    int iflag = _INVALID;
    if (smode_buf[1] == '+' || smode_buf[1] == '-') {
        if (smode_buf[2] == '0') {
            return _INVALID; // Only L0 (invalid)
        }
        if (smode_buf[2] == '\n') { // Only L+
            return _PLUS_ONLY;
        }

        for (int i = 0 ; i < 9 ; i++) { // Check if theres a number after +
            iflag = _VALID;
            if (smode_buf[2] == nums[i]) {
                return _PLUS_CONTINUE; // if true then continue
            } else {
                iflag = _INVALID;
            }
        }
        if (iflag == _INVALID) {
            return _INVALID;
        }
    }

    return _NA;

}

int validate_check_imm(char *smode_buf) { // check immediately for \n, 0 or numbers
/*
_INVALID (immediately stop validating and return invalid to everything)
_IMM_NOTHING == _VALID(valid)
_NA (None of these cases)
_IMM_NUMBER (number immediately after L or N or whatever)
*/
    char nums[] = "123456789";
    int iflag = _INVALID;

    if (smode_buf[1] == '\n') {
        return _IMM_NOTHING;
    }
    if (smode_buf[1] == '0') {
        return _INVALID; 
    }

    for (int j = 0 ; j < 9 ; j++) { // Check if its just a number after
        iflag = _VALID;
        if (smode_buf[1] == nums[j]) {
            return _IMM_NUMBER; 
        } else {
            iflag = _INVALID;
        }
    }
    if (iflag == _INVALID) {
            return _INVALID;
    }

    return _NA; // how did we get here?

}

int validate_plus_continue(char *smode_buf) { // if vcpm returns _PLUS_CONTINUE then run this to further validate

    char nums_with_zero[] = "0123456789";
    int kflag = _INVALID;
    //Start at 2
    for (int i = 2 ; i < SMODE_MAX_INPUT_SIZE ; i++) {
                                            // Nested loop to check every element in nums_with_zero on the current smode element (i)(outer loop)
        
        for (int j = 0 ; j < 10 ; j++) {
            
            if (smode_buf[i] == '\n') {
                return _VALID;
            }
            kflag = _INVALID;
            if (smode_buf[i] == nums_with_zero[j]) {
                kflag = _VALID;
                break;
            }

        } //inner nested loop
        if (kflag == _INVALID) {
            return _INVALID;
        }
    }//outer nested loop

    return _NA;

}

int validate_imm_numbers(char *smode_buf) {

    // Validate after L when its clear that there are only numbers
    char nums_with_zero[] = "0123456789";
    int kflag = _INVALID;
    //Start at 1
    for (int i = 1 ; i < SMODE_MAX_INPUT_SIZE ; i++) {
                                            // Nested loop to check every element in nums_with_zero on the current smode element (i)(outer loop)
        for (int j = 0 ; j < 10 ; j++) {
            
            if (smode_buf[i] == '\n') {
                return _VALID;
            }
            kflag = _INVALID;
            if (smode_buf[i] == nums_with_zero[j]) {
                kflag = _VALID;
                break;
            }

        } //inner nested loop
        if (kflag == _INVALID) {
            return _INVALID;
        }
    }//outer nested loop

    return _NA;
}

int validate_L(char *smode_buf) {

    // this is still a huge function, validating strings is complicated....
    // although i will admit that the code is overall just so much easier to work with. Still happy that i fixed that.

    int vcpm_result = validate_check_p_m(smode_buf);

    int vcimm = FALSE_7ED;
    int plus_continue = TRUE_7ED;
    switch(vcpm_result) {
        case _PLUS_ONLY:
            printf("PLUS ONLY\n");
            return _VALID;
        break;
        case _PLUS_CONTINUE:
            printf("PLUS CONTINUE\n");
        break;
        case _INVALID:
            printf("INVALID\n");
            return _INVALID;
        break;
        case _NA:
            printf("NA\n");
            vcimm = TRUE_7ED;
            plus_continue = FALSE_7ED;
        break;
    }

    if (vcimm == TRUE_7ED) { // This is where L0 is caught
        printf("vcimm start\n");
        int vcimm_result = validate_check_imm(smode_buf);
        int imm_number = FALSE_7ED;
        switch(vcimm_result) {
            case _IMM_NUMBER:
                printf("imm number\n");
                imm_number = TRUE_7ED;
            break;
            case _VALID:
                printf("valid\n");
                return _VALID;
            break;
            case _INVALID:
                printf("invalid\n");
                return _INVALID;
            break;
            case _NA:
                printf("NA\n");
                return _INVALID;
            break;
        }
        if (imm_number == TRUE_7ED) {
            printf("vimmn start\n");
            int vimmn_result = validate_imm_numbers(smode_buf);
            switch(vimmn_result) {
                case _VALID:
                    printf("valid\n");
                    return _VALID;
                break;
                case _INVALID:
                    printf("invalid\n");
                    return _INVALID;
                break;
                case _NA:
                    printf("NA\n");
                    return _INVALID;
                break;
            }
        }
                            // after vcimm we start vimmn
    }

    if (plus_continue == TRUE_7ED) {

        printf("validate plus continue \n");
        int vpct_result = validate_plus_continue(smode_buf);
        switch(vpct_result) {
            case _VALID:
                printf("valid\n");
                return _VALID;
            break;
            case _INVALID:
                printf("invalid\n");
                return _INVALID;
            break;
            case _NA:
                printf("NA\n");
                return _INVALID;
            break;

        }

    }

    // this below will become validate_imm_numbers or something
    
    return _INVALID; // temporary invalid

}

int validate_N(char *smode_buf) {
    /* N will work very similarly to L.
    When i mark as (DONE) i don't actually mean the full feature, i mean the input validation for that specific feature to be done.
    
    Just N will create a new line after the focus line (DONE) 
    N+ will do the same (DONE)
    N+1 will do the same (DONE)
    In short: N = N+ = N+1 (They will all create 1 line after the focus line)

    N+2 Will create 2 lines after focus line. (DONE)
    N+10 Will create 10 lines after the focus line (DONE)
    N10 Will create a line after line 10 (NOT DONE)
    
    N- will work a bit differently. Since N+, N+1, N work the same N wont be available for making lines in the opposite direction.
    I am not even sure if i want '-' to be a valid input. Or how ill even do it. For now i will make all '-'/negatives ivalid. 

    If focus is 1 and we say N-10 Then it will create 10 lines "before" 1 (out of scope for validate_N)

    */
    char nums_with_zero[] = "0123456789";
    char nums[] = "123456789";
    int N_num_flag = _INVALID;
    int plus_num_flag = _INVALID; //If true the if statement will validate if input is valid after the +
    int num_flag = _INVALID; // If true the if statement will validate if input is valid after L
    if (smode_buf[1] == '+') {
        plus_num_flag = _ONE;
        if (smode_buf[2] == '0') {
            return _INVALID;
        }

        if (!((smode_buf[2] >= '0' && smode_buf[2] <= '9') || smode_buf[2] == '\n')) {
            return _INVALID;
        }

        for (int i = 0 ; i < 9 ; i++) { // Check if theres a number after +
            if (smode_buf[2] == nums[i]) {    
                plus_num_flag = _VALID;                 
                break;
            }
        }
    }

    if (plus_num_flag == _VALID) { // Because there is a number after + or - then the flag is 0 and this code runs
        int plus_valid_flag = _INVALID;
        N_num_flag = _INVALID; // False
        // Check the rest with a loop. Start at 2
        for (int i = 2 ; i < SMODE_MAX_INPUT_SIZE ; i++) {
                                                // Nested loop to check every element in nums_with_zero on the current smode element (i)(outer loop)
            for (int j = 0 ; j < 10 ; j++) {

                if(smode_buf[i] == nums_with_zero[j]) {//check if its not a number. It could be the null terminator so we check if it is
                    if (smode_buf[i+1] == '\n' || smode_buf[i+1] == '\0') {
                        N_num_flag = _VALID; // valid
                        plus_valid_flag = _VALID; //valid
                        
                        break;
                    }
                    break;
                }
                if(smode_buf[i+1] != nums_with_zero[j]) {
                    return _INVALID;
                }

            } //inner nested loop
            if (plus_valid_flag == _VALID) {
                break;
            }
            

        }//outer nested loop
    }

    // We aren't checking for N_num_flag yet because i haven't gotten that far in the code
    if (smode_buf[1] == '0') { // [1] because there is no + at 1. Number starts immediately after L
        N_num_flag = _INVALID; // if 0 then its just invalid immediately
    }

    for (int i = 0 ; i < 9 ; i++) { // Check if it starts with a number
        if (smode_buf[1] == nums[i]) {
            //printf("Its a number!\n");
            num_flag = _VALID;
            break;
        }
    }
    
    if (num_flag == 0) { // Start validating if there are numbers after N
        int valid_flag = _INVALID;
        N_num_flag = _INVALID; // False
        int Kflag = _INVALID;
        // Check the rest with a loop. Start at 2
        for (int i = 1 ; i < SMODE_MAX_INPUT_SIZE ; i++) {
                                                // Nested loop to check every element in nums_with_zero on the current smode element (i)(outer loop)
            for (int j = 0 ; j < 10 ; j++) {

                if(smode_buf[i] == nums_with_zero[j]) {//check if its not a number. It could be the null terminator so we check if it is
                    if (smode_buf[i+1] == '\n' || smode_buf[i+1] == '\0') {

                        N_num_flag = _VALID;
                        valid_flag = _VALID;
                        break;
                    }
                    Kflag = _INVALID;
                    for (int k = 0 ; k < 10 ; k++) {
                        if (smode_buf[i+1] == nums_with_zero[k]) { // We checked for newline earlier but since we didnt fine one
                            Kflag = _VALID;                         // We check if theres a number. If theres a number then keep going
                            break;                                  // If there is anything but a number then Return _INVALID
                        }
                    }
                    if (Kflag == _INVALID) {
                        return _INVALID;
                    }
                    break;
                }

            } //inner nested loop
            if (valid_flag == _VALID) {
                //printf("Validflag!\n");
                break;
            }
            
        }//outer nested loop
    }

    if (smode_buf[1] == '\n') {
        return _VALID; // just return N
    }
    
    if (plus_num_flag == _ONE) {
        return _VALID;
    }

    if (N_num_flag == _VALID) {
        return _VALID; // actually return the valid string here
    } 
    if (N_num_flag == _INVALID) {
        return _INVALID; // invalid! return '?'
    }

    return _INVALID;

}