CC=gcc
CFLAGS=-Wfatal-errors -Wall -Werror -Wextra -g -O2 -static
CFLAGS_TESTBIN=-Wfatal-errors -Wall -Werror -Wextra -g -fsanitize=address
TARGET=7ed
TESTTARGET=7ed-TESTING
INSTALL_DIRECTORY=/usr/local/bin
MAKEFLAGS += -s

all: 7ed
7ed:
	echo "CC 7ed.c functions.c startmode.c editmode.c ---> $(TARGET)"
	$(CC) $(CFLAGS) 7ed.c functions.c startmode.c editmode.c -o $(TARGET)
	echo "$(TARGET) is done. Run 'make install' as root to install it"
	
clean:
	echo "rm -f $(TARGET)"
	rm -f $(TARGET)
	echo "rm -f test/$(TESTTARGET)"
	rm -f test/$(TESTTARGET)

tests:
	echo "CC 7ed.c functions.c startmode.c editmode.c ---> test/$(TESTTARGET)"
	$(CC) $(CFLAGS_TESTBIN) 7ed.c functions.c startmode.c editmode.c -o test/$(TESTTARGET)

install:
	cp $(TARGET) $(INSTALL_DIRECTORY)
	echo "$(TARGET) was installed to $(INSTALL_DIRECTORY)"

