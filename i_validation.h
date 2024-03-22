#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "7ed.h"
#include <stdint.h>

#define _PLUS_ONLY _VALID
#define _PLUS_CONTINUE 2
#define _PLUS_NOTHING _VALID
#define _IMM_NUMBER 4
#define _IMM_NOTHING _VALID
#define _CONTINUE 6
#define _NA 7

int validate_imm_numbers(char *smode_buf);

int validate_check_p_m(char *smode_buf);

int validate_check_imm(char *smode_buf);

int validate_plus_continue(char *smode_buf);

int validate_L(char *smode_buf);

int validate_N(char *smode_buf);