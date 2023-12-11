#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "7ed.h"
#include <stdint.h>

int write_line(char filename[], long focus, char editbuffer[], size_t editbuffer_size) {

char *line;
size_t start;
int ret = GET_LINE(filename, focus, &line, &start); // get start position of focus
if (ret == 1) {
    return EXIT_FAILURE;
}

FILE *file;
FILE *temp;
file = fopen(filename,"r"); // Open file
temp = fopen("temp.7ed","w+");
if (file == NULL || temp == NULL) { // Check if you can open file
    fprintf(stderr, "Cannot open file. write_line\n");
    return 1;
}

fseek(file, start, SEEK_SET);
size_t count = 0;
while (1) {
    char c = fgetc(file);
    if (c == '\n') {
        break;               // count the length of the specific line to be replaced
    } else if (c == EOF) {
        break;
    } else {
        count++;
    }
}

fseek(file, 0, SEEK_SET); // go to start of file
size_t counter = 0;
char ch;
for (;; counter++) {  // for loop that puts contents of file in to temp
                      // when counter is equal to start, the buffer with the edited content is written to temp
    if (start == counter) { // after that it uses fseek to point temp right after the written content
        fprintf(temp, "%s", editbuffer); // and fseek on file will use count variable to skip the content that is supposed to be replaced
        fseek(temp, start+editbuffer_size-1, SEEK_SET); // After that the function keeps writing everything else to temp
        fseek(file, start+count, SEEK_SET);
    }
    ch = fgetc(file);
    if (ch == EOF) {
        break;
    }
    fputc(ch, temp);

}
fseek(temp, 0, SEEK_SET); // reset both files to start at 0 so it doesnt mess with fread and fwrite
fclose(file);
file = fopen(filename, "w");
if (file == NULL) { // Check if you can open file
    fprintf(stderr, "Cannot open file for writing.\n");
    return 1;
}
char buffer[BUF_SZ_2];
size_t bytes_read;
while ((bytes_read = fread(buffer, 1, BUF_SZ_2, temp)) > 0) { // Write contents of temp to file
    fwrite(buffer, 1, bytes_read, file);
}
free(line);
fclose(file);
fclose(temp);
remove("temp.7ed");
return 0;

}


int NEW_LINE(char filename[]) { // doin this test again

    long new_line_pos; // The "focus" that the newline will be inserted afterwards
    long new_line_pos_temp; // temp
    char buf[1024];
    int success;

    do {
        fprintf(stdout, "Create a new line after: ");
        if (!fgets(buf, 1024, stdin)) {    // take input from user
            fprintf(stderr, "Too many characters\n");
            break;
        }
        char *endptr;

        new_line_pos_temp = strtol(buf, &endptr, 10);
        errno = 0;
        if (errno == ERANGE) {
            fprintf(stderr, "Sorry, this number is too small or too large.\n");
            success = 0;
        }
        else if (endptr == buf) {
            // no character was read
            success = 0;
        }
        else if (*endptr && *endptr != '\n') {
            success = 0;
        }

        else {
            success = 1;
        }

        } while (!success);

        if (new_line_pos_temp < 0) {
            fprintf(stderr, "The new line can not be under 0!\n");
            return 1;
        } else {
            new_line_pos = new_line_pos_temp;

            char *line; // line is the line before the newline
                                // line2 is the line right after line
            size_t start; // start is the position at the start of line
                                  // start2 is the position at the start of line2
            int ret = GET_LINE(filename, new_line_pos+1, &line, &start);
            if (ret == 1) {
                return 1;
            }

            size_t len = strlen(line);
            char to_be_written[len+2];
            to_be_written[0] = '\n';
            to_be_written[1] = '\0';
            strcat(to_be_written, line);
            
            write_line(filename, new_line_pos+1, to_be_written, len+1);
            free(line);
        }

    return 0;

}

int editmode(char filename[], long focus) {

char *line;
size_t start;
int ret = GET_LINE(filename, focus, &line, &start);
if (ret == 1) {
    return EXIT_FAILURE;
}

fprintf(stdout, "%s", line); // print line to be edited
free(line);

while (1) {
    char editbuffer[BUF_SZ_2];
    fprintf(stdout, "(%ld EDIT): ", focus);

    fgets(editbuffer, BUF_SZ_2, stdin);

    if (editbuffer[0] == '\n') { continue; }

    fprintf(stdout, "Do you want to write the changes?\n");
    int yesno = CHOICE();

    if (yesno == 1) {
        printf("N\n");
        return 0;
    }
    
    size_t editbuffer_size = strlen(editbuffer);
    write_line(filename, focus, editbuffer, editbuffer_size);

    return 0;

}

return EXIT_SUCCESS;

}