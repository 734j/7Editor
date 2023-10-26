all: 7ed
7ed:
	gcc -Wfatal-errors -Wall -Werror -Wextra -g 7ed.c functions.c startmode.c -o 7ed
clean:
	rm -f 7ed