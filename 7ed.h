#include <stddef.h>

#define BUF_SZ_256 256
#define BUF_SZ_512 512
#define BUF_SZ_1 1024
#define BUF_SZ_2 2048
#define BUF_SZ_4 4096
#define BUF_SZ_8 8192

#ifndef COUNT_LINES_IN_FILE_H
#define COUNT_LINES_IN_FILE_H

int COUNT_LINES_IN_FILE (char filename[], size_t *lines);

#endif /* COUNT_LINES_IN_FILE_H */

#ifndef COUNT_LINES_IN_FILE_POSIX_H
#define COUNT_LINES_IN_FILE_POSIX_H

int COUNT_LINES_IN_FILE_POSIX (char filename[], size_t *lines);

#endif /* COUNT_LINES_IN_FILE_POSIX_H */

#ifndef CONFIRM_H
#define CONFIRM_H

void CONFIRM();

#endif /* CONFIRM_H */

#ifndef CHOICE_H
#define CHOICE_H

int CHOICE();

#endif /* CHOICE_H */

#ifndef SHUFFLE_H
#define SHUFFLE_H

void shuffle(char arr[], int n);

#endif /* SHUFFLE_H */

#ifndef STARTMODE_H
#define STARTMODE_H
int startmode(char filename[]);

#endif /* STARTMODE_H */


#ifndef GET_LINE_H
#define GET_LINE_H

int GET_LINE(char filename[], long focus, char **line, size_t *start);

#endif /* GET_LINE_H */

#ifndef EDITMODE_H
#define EDITMODE_H

int editmode(char filename[], long focus);

#endif /* EDITMODE_H */

#ifndef NEW_LINE_H
#define NEW_LINE_H

int NEW_LINE(char filename[]);

#endif /* NEW_LINE_H */