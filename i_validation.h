#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "7ed.h"
#include <stdint.h>

#define _PLUS_ONLY 1
#define _PLUS_CONTINUE 2
#define _PLUS_NOTHING 3
#define _IMM_NUMBER 4
#define _IMM_NOTHING 5
#define _CONTINUE 6
#define _NA 7

int validate_L(char *smode_buf);

int validate_N(char *smode_buf);