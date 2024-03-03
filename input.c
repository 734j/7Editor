#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "7ed.h"
#include <stdint.h>

#define _SINGLE 1
#define _MULTIPLE 2 
#define _FAIL '?' // final return value from smode_input to indicate an invalid
#define _VALID 0 // this may only be used to mark as valid or invalid from the validate functions
#define _INVALID -1 // this may only be used to mark as valid or invalid from the validate functions
                    // _FAIL and _INVALID are sorta tied to eachother

// This will be the new input system for combining commands with line numbers
// Work in progress and far from finished. This is not included when compiling normally.

#define SMODE_MAX_SIZE 33
#define SMODE_MAX_INPUT_SIZE 32

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
    int plus_num_flag = -1; //If true the if statement will validate if input is valid after the +
    int num_flag = -1; // If true the if statement will validate if input is valid after L

    if (smode_buf[1] == '+' || smode_buf[1] == '-') {
        //printf("Its a plus!\n");
        plus_num_flag = 1;
        // Check if its a plus
        if (smode_buf[2] == '0') {
            L_num_flag = -1; // if 0 then its just invalid immediately
        }
        if (smode_buf[2] != '\n') { // If there is no return we set these flags to false. 
            plus_num_flag = -1;     // If there is a return then no flags are changed and thus we know that the input is only an L+
            L_num_flag = -1;
        }

        for (int i = 0 ; i < 9 ; i++) { // Check if theres a number after +
            if (smode_buf[2] == nums[i]) {
                //printf("Its a number after + \n");
                plus_num_flag = 0;
                break;
            }
        }
    }


    if (plus_num_flag == 0) {
        int plus_valid_flag = -1;
        L_num_flag = -1; // False
        char nums_with_zero[] = "0123456789";
        // Check the rest with a loop. Start at 2
        for (int i = 1 ; i < SMODE_MAX_INPUT_SIZE ; i++) {
                                                // Nested loop to check every element in nums_with_zero on the current smode element (i)(outer loop)
            for (int j = 0 ; j < 10 ; j++) {

                if(smode_buf[i] == nums_with_zero[j]) {//check if its not a number. It could be the null terminator so we check if it is
                    if (smode_buf[i+1] == '\n' || smode_buf[i+1] == '\0') {
                        L_num_flag = 0;
                        plus_valid_flag = 0;
                        //printf(" +++ String is valid!\n");
                        // success! return the "valid" string. (example: L44)
                        // Do something to indicate that its successful and break out somehow not sure yet
                        break;
                    }
                    break;
                }

            } //inner nested loop
            if (plus_valid_flag == 0) {
                //printf(" +++ Validflag!\n");
                break;
            }
            

        }//outer nested loop
    }

    if (smode_buf[1] == '0') { // [1] because there is no + at 1. Number starts immediately after L
        L_num_flag = -1; // if 0 then its just invalid immediately
        if(smode_buf[2] == '\n') {
            //valid
            L_num_flag = 0;
        }
    }

    for (int i = 0 ; i < 9 ; i++) { // Check if it starts with a number
        if (smode_buf[1] == nums[i]) {
            //printf("Its a number!\n");
            num_flag = 0;
            break;
        }
    }
    
    if (num_flag == 0) { // Start validating if there are numbers after L
        int valid_flag = -1;
        L_num_flag = -1; // False
        char nums_with_zero[] = "0123456789";
        // Check the rest with a loop. Start at 2
        for (int i = 1 ; i < SMODE_MAX_INPUT_SIZE ; i++) {
                                                // Nested loop to check every element in nums_with_zero on the current smode element (i)(outer loop)
            for (int j = 0 ; j < 10 ; j++) {

                if(smode_buf[i] == nums_with_zero[j]) {//check if its not a number. It could be the null terminator so we check if it is
                    if (smode_buf[i+1] == '\n' || smode_buf[i+1] == '\0') {

                        // 0 1 2 3
                        // L 2 5 8
                        L_num_flag = 0;
                        valid_flag = 0;
                        //printf("Success! String is valid!\n");
                        // success! return the "valid" string. (example: L44)
                        // Do something to indicate that its successful and break out somehow not sure yet
                        break;
                    }
                    break;
                }

            } //inner nested loop
            if (valid_flag == 0) {
                //printf("Validflag!\n");
                break;
            }
            

        }//outer nested loop
    }// num_flag if-statement
    if (L_num_flag == 0) {
        //printf("Because the string was valid we return it here!\n");
        // actually return the valid string here
        return _VALID;
    } 
    if (L_num_flag == -1) {
        //printf("Because the string was invalid we return '?' here!\n");
        // invalid! return '?'
        return _INVALID;
    }

    if (plus_num_flag == 1) {
        //printf("L+ only!");
        return _VALID;
    }

    if (smode_buf[1] == '\n') {
        //printf("L only!\n");
        // just return L
        return _VALID;
    }

    //printf("Because the string was invalid we return '?' here!\n"); // Last catch, if it makes it to here then just invalid
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
        case 'L':
            
            int chk = validate_L(smode_buf);
            if (chk == _INVALID) {
                return _FAIL;
            }
            *multiple = (char *)malloc(strlen(smode_buf) + 1); // just a test
            strcpy(*multiple, smode_buf);
            return _MULTIPLE;

        break;
        case 'n':
        case 'N':
            return _MULTIPLE;
        break;
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
    
    // invalid inputs
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
    //fprintf(stdout, "%s\n", multiple);
    //free(multiple);

}