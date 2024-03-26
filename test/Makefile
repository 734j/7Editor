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
	rm -f vcpm
	echo "rm -f vcpm"
	rm -f vcimm
	echo "rm -f vcimm"
	rm -f vpct
	echo "rm -f vpct"
	rm -f vimmn
	echo "rm -f vimmn"
	echo "Cleaned."

smode:
	echo "CC input.c ---> smode"
	cd .. && $(CC) $(CFLAGS_SMODE) input.c i_validation.c -o test/smode

alltests:
	cd .. && make tests
	$(CC) $(CFLAGS_ALLTESTS) validate_check_p_m.c ../i_validation.c -o vcpm
	echo "CC ---> vcpm" 
	$(CC) $(CFLAGS_ALLTESTS) validate_check_imm.c ../i_validation.c -o vcimm
	echo "CC ---> vcimm" 
	$(CC) $(CFLAGS_ALLTESTS) validate_plus_continue_tests.c ../i_validation.c -o vpct
	echo "CC ---> vpct" 
	$(CC) $(CFLAGS_ALLTESTS) validate_imm_numbers.c ../i_validation.c -o vimmn
	echo "CC ---> vimmn" 
