#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "7ed.h"
#include <stdint.h>

extern uint8_t new;
extern uint8_t g_choicemode;

int delete_specified_newline(char filename[], long focus) { // special version of write_line that does as the name says

    char *line;
    size_t start;
    int ret = get_line(filename, focus, &line, &start); // get start position of focus
    if (ret == 1) {
        return EXIT_FAILURE;
    }

    start = start-1; // decrement by 1 to place start position at the newline

    FILE *file;
    FILE *temp;
    file = fopen(filename,"r"); // Open file
    temp = fopen("temp.7ed","w+"); // Open temp file
    if (file == NULL || temp == NULL) { // Check if you can open file
        fprintf(stderr, "Cannot open file. delete_specified_newline\n");
        return 1;
    }

    fseek(file, 0, SEEK_SET); // go to start of file
    size_t counter = 0;
    char ch;
    for (;; counter++) {  // for loop that puts contents of file in to temp
                        // when counter is equal to start, the buffer with the edited content is written to temp
        if (start == counter) { // after that it uses fseek to point temp right after the written content
            
            fseek(temp, start, SEEK_SET); // After that the function keeps writing everything else to temp
            fseek(file, start+1, SEEK_SET); // +1 to skip over the newline and thus having it be removed
        }
        ch = fgetc(file);
        if (ch == EOF) {
            break;
        }
        fputc(ch, temp);

    }
    fseek(temp, 0, SEEK_SET); // reset both files to start at 0 so it doesnt mess with fread and fwrite
    fclose(file);
    file = fopen(filename, "w"); // open file again to write the rest
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

int write_line(char filename[], long focus, char editbuffer[], size_t editbuffer_size) { // writes text at specified line

    char *line;
    size_t start;
    int ret = get_line(filename, focus, &line, &start); // get start position of focus
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

int check_end_newline(char filename[]) { // function that checks if a file ends with a newline

    FILE *file; 
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file. check_end_newline()\n");
        return 1;
    }

    fseek(file, -1, SEEK_END);
    if (fgetc(file) != '\n') {
        return -1; // returns -1 if there is not newline at the end
    }
    fclose(file);
    return 0; // returns 0 if there is one

}

int new_line(char filename[], long long new_line_pos_temp, uint64_t amount_of_lines) { // creates a new line within a file after a specified line number

        long long new_line_pos;
        if (new_line_pos_temp < 0) {
            fprintf(stderr, "The new line can not be under 0!\n");
            return 1;
        } else {
            new_line_pos = new_line_pos_temp;

            int cen = check_end_newline(filename);
            if (cen == -1) {
                size_t linecount;
                int clif = count_lines_in_file(filename, &linecount);
                if (clif == 1) {
                    return 1;
                }
                if (linecount == (size_t)new_line_pos) { // if the file ends without a newline and the user wants to put a newline at the end, then this code within the if-statement will do that.
                    FILE *file;
                    file = fopen(filename, "a");
                    if (file == NULL) {
                        fprintf(stderr, "Cannot open file. new_line()\n");
                        return 1;
                    }
                    fputc('\n', file);
                    fputc('\n', file);
                    fclose(file);
                    return 0;
                }

            }
        
            char *line; // line is the line before the newline
                                // line2 is the line right after line
            size_t start; // start is the position at the start of line
                                  // start2 is the position at the start of line2
            int ret = get_line(filename, new_line_pos+1, &line, &start);
            if (ret == 1) {
                return 1;
            }

            size_t len = strlen(line);

            // Get length of line
            // Get length of amount of newlines
            // put them both in a buffer
            // Input the buffer into write_line
            size_t total_size = amount_of_lines+len+2;
            char *to_be_written = malloc(total_size);
            uint64_t i = 0;
             for ( ; i < amount_of_lines ; i++) {
                to_be_written[i] = '\n';
            }
            to_be_written[i+1] = '\0';

            strcat(to_be_written, line);
            
            write_line(filename, new_line_pos+1, to_be_written, total_size-1);
            free(line);
            free(to_be_written);
        }

    return 0;

}

int remove_line_contents(char *filename, uint64_t focus) { // removes contents of a specified line without removing the newline

    char *line;
    size_t start;
    int ret = get_line(filename, focus, &line, &start);
    if (ret == 1) {
        return 1;
    }
    
    int cen = check_end_newline(filename); // check if file ends without newline
                                           // check_end_newline returns 0 if ther is a newline
                                           // it returns -1 if ther is none
    char editbuffer[1] = {'\0'};
    write_line(filename, focus, editbuffer, 1); // remove contents in line

    if (cen == -1) { // put newline at end if cen == -1
        new_line(filename, focus-1, 1);
    }
    free(line);
    return 0;

}

int remove_line_contents_and_newline(char *filename, uint64_t focus) {

    int increment = 0;
    if (focus == 1) { // checks if its line 1. This is so that we can remove the newline properly.
        increment++;
    }
    int rlc = remove_line_contents(filename, focus);
    if (rlc == 1) {
        return 1;
    }

    int dsn = delete_specified_newline(filename, focus+increment);
    if (dsn == 1) {
        return 1;
    }

    return 0;
}

int editmode(char filename[], uint64_t focus) { // the editing interface

    char *line;
    size_t start;
    int ret = get_line(filename, focus, &line, &start);
    if (ret == 1) {
        return EXIT_FAILURE;
    }

    fprintf(stdout, "%s", line); // print line to be edited
    free(line);

    while (1) {
        char editbuffer[BUF_SZ_2];
        fprintf(stdout, "(%ld EDIT): ", focus);

        char *fgs = fgets(editbuffer, BUF_SZ_2, stdin);
	if (fgs == NULL) {
	    return 0;
	}

        if (editbuffer[0] == '\n') { 
            // No changes
            return 0;
        }

        int yesno = choice(g_choicemode);

        if (yesno == 1) {
            return 0;
        }
        
        new = 0;
        size_t editbuffer_size = strlen(editbuffer);
        write_line(filename, focus, editbuffer, editbuffer_size);

        return 0;

    }

    return EXIT_SUCCESS;

}
