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
#define MODE_L 76 // MODE_L works for Commands with + and -
#define MODE_N 78 // MODE_N works for Commands with + only

int validate_imm_numbers(char *smode_buf, int mode);

int validate_check_p_m(char *smode_buf, int mode);

int validate_check_imm(char *smode_buf, int mode);

int validate_plus_continue(char *smode_buf);

int validate_LN(char *smode_buf, int mode);