#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "../7ed.h"
#include "../i_validation.h"
#include "../input.h"
#include <stdint.h>

// Test for validate_check_imm

int main () {
    uint64_t focus = 1;
    char smode_buf[SMODE_MAX_SIZE]; // Smode buffer
    fprintf(stdout, "(%lu): ", focus); // UI
    fgets(smode_buf, SMODE_MAX_SIZE, stdin); // Read user input

    int ret = validate_check_imm(smode_buf);

    if(ret == _INVALID) {
        printf("?\n");
    }
    if(ret == _VALID) {
        printf("valid plusonly\n");
    }

    return 0;

}