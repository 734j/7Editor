MAKEFLAGS += -s

all: 7ed-TESTING
7ed-TESTING:
	cd .. && make tests
	echo "Done."

clean: 
	cd .. && make clean
	echo "Cleaned."

smode:
	echo "CC input.c ---> smode"
	cd .. && gcc -Wfatal-errors -Wall -Werror -Wextra -g -fsanitize=address input.c -o test/smode
