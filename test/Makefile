MAKEFLAGS += -s

all: 7ed-TESTING
7ed-TESTING:
	cd .. && make tests
	echo "Done."

clean: 
	cd .. && make clean
	rm -f vcpm
	echo "rm -f vcpm"
	rm -f vcimm
	echo "rm -f vcpm"
	rm -f vpct
	echo "rm -f vcpm"
	echo "Cleaned."

smode:
	echo "CC input.c ---> smode"
	cd .. && gcc -Wfatal-errors -Wall -Werror -Wextra -g -fsanitize=address input.c i_validation.c -o test/smode

alltests:
	cd .. && make tests
	gcc validate_check_p_m.c ../i_validation.c -o vcpm
	echo "CC ---> vcpm" 
	gcc validate_check_imm.c ../i_validation.c -o vcimm
	echo "CC ---> vcimm" 
	gcc validate_plus_continue_tests.c ../i_validation.c -o vpct
	echo "CC ---> vpct" 