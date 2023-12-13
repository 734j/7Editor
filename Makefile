all: 7ed
7ed:
	gcc -Wfatal-errors -Wall -Werror -Wextra -g -O2 7ed.c functions.c startmode.c editmode.c -o 7ed
	
clean:
	rm -f 7ed
	rm -f test/7ed-TESTING

tests:
	gcc -Wfatal-errors -Wall -Werror -Wextra -g -fsanitize=address 7ed.c functions.c startmode.c editmode.c -o test/7ed-TESTING
	
