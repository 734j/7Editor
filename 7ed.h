#include <stddef.h>
#include <stdint.h>

#define BUF_SZ_256 256
#define BUF_SZ_512 512
#define BUF_SZ_1 1024
#define BUF_SZ_2 2048
#define BUF_SZ_4 4096
#define BUF_SZ_8 8192



int COUNT_LINES_IN_FILE (char filename[], uint64_t *lines);

int COUNT_LINES_IN_FILE_POSIX (char filename[], size_t *lines);

void CONFIRM();

int CHOICE();

void shuffle(char arr[], int n);

int startmode(char filename[]);

int GET_LINE(char filename[], long focus, char **line, size_t *start);

int editmode(char filename[], long focus);

int NEW_LINE(char filename[]);
