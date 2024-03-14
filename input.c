#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "7ed.h"
#include "input.h"
#include <stdint.h>


// This will be the new input system for combining commands with line numbers
// Work in progress and far from finished. This is not included when compiling normally.

int validate_L(char *smode_buf) {

    /*
            Check for + (or -)
            path 1: If there is a + then check if there is anything else after it. If not then return L+. (DONEish)
            1.1: If there is a anything that is not a number from 0-9 then return '?' (safety check)
            1.2: If there is a number after the + then check for another number until there is no more numbers. For example we return L+34
            1.3: If there happens to be something that is not a number from 0-9 in 1.2 case then immediately return '?' (safety check)
            
            path 2: If there is a number from 1-9 immediately after the L then keep checking for numbers (between 0-9) until there is no more. (DONE)
            A valid return would be something like L24 (DONE)
            2.1: If there is anything that is not a number from 1-9 then return '?' (DONE)

            
            path 3: If there is nothing else after the L (in this case just an L) then just return an L (DONE)
            */

    char nums[] = "123456789";
    int L_num_flag = 2;
    int plus_num_flag = _INVALID; //If true the if statement will validate if input is valid after the +
    int num_flag = _INVALID; // If true the if statement will validate if input is valid after L

    if (smode_buf[1] == '+' || smode_buf[1] == '-') {
        plus_num_flag = _ONE;
        if (smode_buf[2] == '0') {
            L_num_flag = _INVALID; // if 0 then its just invalid immediately
        }
        if (smode_buf[2] != '\n') { // If there is no return we set these flags to false. 
            plus_num_flag = _INVALID;     // If there is a return then no flags are changed and thus we know that the input is only an L+
            L_num_flag = _INVALID;
        }

        for (int i = 0 ; i < 9 ; i++) { // Check if theres a number after +
            if (smode_buf[2] == nums[i]) {
                plus_num_flag = _VALID;
                break;
            }
        }
    }


    if (plus_num_flag == _VALID) {
        int plus_valid_flag = _INVALID;
        L_num_flag = _INVALID; // False
        char nums_with_zero[] = "0123456789";
        // Check the rest with a loop. Start at 2
        for (int i = 2 ; i < SMODE_MAX_INPUT_SIZE ; i++) {
                                                // Nested loop to check every element in nums_with_zero on the current smode element (i)(outer loop)
            for (int j = 0 ; j < 10 ; j++) {
                if(smode_buf[i] == nums_with_zero[j]) {//check if its not a number. It could be the null terminator so we check if it is
                    if (smode_buf[i+1] == '\n' || smode_buf[i+1] == '\0') {
                        L_num_flag = _VALID;
                        plus_valid_flag = _VALID;
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

    if (smode_buf[1] == '0') { // [1] because there is no + at 1. Number starts immediately after L
        L_num_flag = _INVALID; // if 0 then its just invalid immediately
    }

    for (int i = 0 ; i < 9 ; i++) { // Check if it starts with a number
        if (smode_buf[1] == nums[i]) {
            num_flag = _VALID;
            break;
        }
    }
    
    if (num_flag == 0) { // Start validating if there are numbers after L
        int valid_flag = _INVALID;
        L_num_flag = _INVALID; // False
        int Kflag; 
        char nums_with_zero[] = "0123456789";
        // Check the rest with a loop. Start at 2
        for (int i = 1 ; i < SMODE_MAX_INPUT_SIZE ; i++) {
                                                // Nested loop to check every element in nums_with_zero on the current smode element (i)(outer loop)
            for (int j = 0 ; j < 10 ; j++) {
                if(smode_buf[i] == nums_with_zero[j]) {//check if there is a number, if there is then next check
                    if (smode_buf[i+1] == '\n' || smode_buf[i+1] == '\0') { // if there was a number then check if theres a newline
                        L_num_flag = _VALID;                                // if we had a newline then check the flags as valid and break out.
                        valid_flag = _VALID;                                // it will break out here and then an if statement check will also check it again so that it can break out of the outer loop
                    
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
                break;
            }
            

        }//outer nested loop
    }// num_flag if-statement
    if (L_num_flag == _VALID) {
        return _VALID;
    } 
    if (L_num_flag == _INVALID) {
        return _INVALID;
    }

    if (plus_num_flag == _ONE) {
        return _VALID;
    }

    if (smode_buf[1] == '\n') {
        return _VALID;
    }

    return _INVALID;
    
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

int smode_input(char *single, char **multiple, uint64_t focus) { // This function is for input then calls the appropriate validator 

    // char *single is for p, e, c, q, a
    // char **multiple is for L, n, x and d. Although it can be expanded to be used in p and e. 

    char smode_buf[SMODE_MAX_SIZE]; // Smode buffer
    fprintf(stdout, "(%lu): ", focus); // UI
    fgets(smode_buf, SMODE_MAX_SIZE, stdin); // Read user input
    
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