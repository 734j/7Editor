CC=gcc
CFLAGS_TESTBIN=-O3 -Wfatal-errors -Wall -Werror -Wextra -g -fsanitize=address -Wpedantic -std=gnu11
CFLAGS=-O3 -flto -march=native -DNDEBUG -fomit-frame-pointer -s -static -std=gnu11
TARGET=7ed
TESTTARGET=7ed-TESTING
INSTALL_DIRECTORY=/usr/local/bin
MAKEFLAGS += -s
SRCS=7ed.c functions.c startmode.c editmode.c input.c i_validation.c process_multiples.c

all: release
clean:
	echo "rm -f $(TARGET)"
	rm -f $(TARGET)
	echo "rm -f test/$(TESTTARGET)"
	rm -f test/$(TESTTARGET)
	echo "rm -f test/smode"
	rm -f test/smode

tests:
	echo "CC 7ed.c functions.c startmode.c editmode.c i_validation.c process_multiples.c ---> test/$(TESTTARGET)"
	$(CC) $(CFLAGS_TESTBIN) $(SRCS) -o test/$(TESTTARGET)

install:
	cp $(TARGET) $(INSTALL_DIRECTORY)
	echo "$(TARGET) was installed to $(INSTALL_DIRECTORY)"

release:
	echo "CC 7ed.c functions.c startmode.c editmode.c i_validation.c process_multiples.c ---> $(TARGET)"
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)
	echo "$(TARGET) is done."
