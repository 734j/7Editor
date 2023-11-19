all: 7ed
7ed:
	gcc -Wfatal-errors -Wall -Werror -Wextra -g -fsanitize=address 7ed.c functions.c startmode.c editmode.c -o 7ed
	cp 7ed test
	mv test/7ed test/7ed-test
clean:
	rm -f 7ed
	rm -f test/7ed-test
