MAKEFLAGS += -s
CFLAGS_ALLTESTS=-fsanitize=address -Wfatal-errors -Wall -Wextra -g
CFLAGS_SMODE=-fsanitize=address -Wfatal-errors -Wall -Werror -Wextra -g
CC=gcc

all: 7ed-TESTING
7ed-TESTING:
	cd .. && make tests
	echo "Done."

clean: 
	cd .. && make clean
	echo "Cleaned."

smode:
	echo "CC input.c ---> smode"
	cd .. && $(CC) $(CFLAGS_SMODE) input.c i_validation.c -o test/smode

alltests:
	cd .. && make tests
	echo "Done."
