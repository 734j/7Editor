#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define BUF_SZ_256 256
#define BUF_SZ_512 512
#define BUF_SZ_1 1024
#define BUF_SZ_2 2048
#define BUF_SZ_4 4096
#define BUF_SZ_8 8192

#define TRUE_7ED 0
#define FALSE_7ED 1

#define FILE_EXISTS 0
#define FILE_NOT_FOUND 1

#define MODE_YES 0
#define MODE_NO 1
#define MODE_NORMAL 2

int count_lines_in_file(char filename[], uint64_t *lines);

int count_lines_in_file_posix(char filename[], size_t *lines);

void confirm();

int choice(uint8_t mode);

void shuffle(char arr[], int n);

int startmode(char filename[]);

int get_line(char *filename, long focus, char **line, size_t *start);

int editmode(char filename[], uint64_t focus);

int new_line(char filename[], long long new_line_pos_temp, uint64_t amount_of_lines);

int remove_line_contents(char filename[], uint64_t focus);

int delete_specified_newline(char filename[], long focus);

int remove_line_contents_and_newline(char *filename, uint64_t focus);