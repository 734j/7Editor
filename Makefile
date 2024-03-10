CC=gcc
CFLAGS_TESTBIN=-Wfatal-errors -Wall -Werror -Wextra -g -fsanitize=address
CFLAGS=-O2 -flto -march=native -DNDEBUG -fomit-frame-pointer -s -static
TARGET=7ed
TESTTARGET=7ed-TESTING
INSTALL_DIRECTORY=/usr/local/bin
MAKEFLAGS += -s

all: release
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

release:
	echo "CC 7ed.c functions.c startmode.c editmode.c ---> $(TARGET)"
	$(CC) $(CFLAGS) 7ed.c functions.c startmode.c editmode.c -o $(TARGET)
	echo "$(TARGET) is done."
