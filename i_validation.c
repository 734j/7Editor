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

int validate_check_p_m(char *smode_buf, int mode) { // Check for +, -, +0, -0, +\n, or -\n
/*
_INVALID (immediately stop validating and return invalid to everything)
_PLUS_ONLY (L+ only. Immediately valid. Return)
_PLUS_CONTINUE (L+ And more numbers after it. Will make validate_plus_continue run)
_NA (None of these cases)
*/
    char nums[] = "123456789";
    int iflag = _INVALID;

    switch(mode) {
        case MODE_L:
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
        break;
        case MODE_N:
            if (smode_buf[1] == '+') {
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
        break;
        default:
            fprintf(stderr, "WRONG MODE\n");
            return _INVALID;
    }

    return _NA;

}

int validate_check_imm(char *smode_buf, int mode) { // check immediately for \n, 0 or numbers
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
    if (smode_buf[1] == '0') { // If we are on MODE_N then it has to allow '0' and thus we call it valid.
        if (mode == MODE_N) {  // It will keep validating this in validate_imm_numbers.
            return _IMM_NUMBER;
        } else {
            return _INVALID; 
        }
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

int validate_imm_numbers(char *smode_buf, int mode) {

    // Validate after L when its clear that there are only numbers
    char nums_with_zero[] = "0123456789";
    int kflag = _INVALID;

    if (mode == MODE_N && smode_buf[1] == '0') { // Because we are on MODE_N this runs to check if there is anything after N0. We will only allow newline characters because that means theres ONLY 'N0*
        printf("vimmn mode N\n");
        if (smode_buf[2] == '\n') {
            return _VALID;
        } else {
            return _INVALID; // If there is anything else then it just exits.
        }
    }

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

int validate_LN(char *smode_buf, int mode) {

    // this is still a huge function, validating strings is complicated....
    // although i will admit that the code is overall just so much easier to work with. Still happy that i fixed that.

    int vcpm_result = validate_check_p_m(smode_buf, mode);

    int vcimm = FALSE_7ED;
    int plus_continue = TRUE_7ED;
    switch(vcpm_result) {
        case _PLUS_ONLY:
            //printf("PLUS ONLY\n");
            return _VALID;
        break;
        case _PLUS_CONTINUE:
            //printf("PLUS CONTINUE\n");
        break;
        case _INVALID:
            //printf("INVALID\n");
            return _INVALID;
        break;
        case _NA:
            //printf("NA\n");
            vcimm = TRUE_7ED;
            plus_continue = FALSE_7ED;
        break;
    }

    if (vcimm == TRUE_7ED) { // This is where L0 is caught
        //printf("vcimm start\n");
        int vcimm_result = validate_check_imm(smode_buf, mode);
        int imm_number = FALSE_7ED;
        switch(vcimm_result) {
            case _IMM_NUMBER:
                //printf("imm number\n");
                imm_number = TRUE_7ED;
            break;
            case _VALID:
                //printf("valid\n");
                return _VALID;
            break;
            case _INVALID:
                //printf("invalid\n");
                return _INVALID;
            break;
            case _NA:
                //printf("NA\n");
                return _INVALID;
            break;
        }
        if (imm_number == TRUE_7ED) {
            //printf("vimmn start\n");
            int vimmn_result = validate_imm_numbers(smode_buf, mode);
            switch(vimmn_result) {
                case _VALID:
                    //printf("valid\n");
                    return _VALID;
                break;
                case _INVALID:
                    //printf("invalid\n");
                    return _INVALID;
                break;
                case _NA:
                    //printf("NA\n");
                    return _INVALID;
                break;
            }
        }
                            // after vcimm we start vimmn
    }

    if (plus_continue == TRUE_7ED) {

        //printf("validate plus continue \n");
        int vpct_result = validate_plus_continue(smode_buf);
        switch(vpct_result) {
            case _VALID:
                //printf("valid\n");
                return _VALID;
            break;
            case _INVALID:
                //printf("invalid\n");
                return _INVALID;
            break;
            case _NA:
                //printf("NA\n");
                return _INVALID;
            break;

        }
    }
    
    return _INVALID; // temporary invalid

}
