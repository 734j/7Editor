#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "7ed.h"
#include <stdint.h>

#define SMODE_MAX_SIZE 33
#define SMODE_MAX_INPUT_SIZE 32

#define _ONE 1
#define _SINGLE 1
#define _MULTIPLE 2 
#define _FAIL '?' // final return value from smode_input to indicate an invalid
#define _VALID 0 // this may only be used to mark as valid or invalid from the validate functions
#define _INVALID -1 // this may only be used to mark as valid or invalid from the validate functions
                    // _FAIL and _INVALID are sorta tied to eachother