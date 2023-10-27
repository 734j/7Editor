#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include "7ed.h"
#include <time.h>
#include <string.h>

void CONFIRM() {
    struct termios old,new;

    tcgetattr(fileno(stdin),&old); // gets something?
    tcgetattr(fileno(stdin),&new); // gets something else?
    cfmakeraw(&new); // makes new terminal settings
    tcsetattr(fileno(stdin),TCSANOW,&new); // sets those settings immediately (TCSANOW) to &new
    fputs("Press any key to continue...",stdout);
    fflush(NULL);
    fgetc(stdin); 
    tcsetattr(fileno(stdin),TCSANOW,&old); // goes back to old settings
    puts(""); // newline
}

int CHOICE() {
    int choice;

    do {

    fputs("[Y / N] ? ", stdout);

    choice = getchar();
    if (choice == '\n') { continue; }

    while ('\n' != getchar());

    } while ( (choice != 'Y') && (choice != 'y') && (choice != 'N') && (choice != 'n') );

    if ( (choice == 'Y') || (choice == 'y') ) 
    {
        return 0;
    }

    if ((choice == 'N') || (choice == 'n') )
    {
        return 1;
    }

    return EXIT_FAILURE;
}

int COUNT_LINES_IN_FILE (char filename[], size_t *lines) {

    // Does not follow posix because this function accounts for if the last line does not end with a newline.

    size_t line_count = 0; // Counter starting at 0
    FILE *file;
    file = fopen(filename,"rb"); // Open file

    if (file == NULL) { // Check if you can open file
        fprintf(stderr, "Cannot open file.\n");
        return 1;
    }
    fseek(file, -1, SEEK_END);
    int last = fgetc(file);
    if (last != '\n') {         // Checks if the file ends with a newline or not. If not then it adds 1 to the count to account for the last line
        line_count++;
    } 
    fseek(file, 0, SEEK_SET);

    char buffer[BUF_SZ_4]; // Creates buffer with size of BUF_SZ_4
    while (1) {
        size_t bytes_read = fread(buffer, 1, BUF_SZ_4, file); // puts chars from file in to buffer and returns the amount of bytes.
        for (size_t i = 0 ; i < bytes_read; i++) { 
            if (buffer[i] == '\n') { // Searches through the buffer for newlines.
                line_count++;
            } 
        }
        if (feof(file)) { // If end of file is encountered then break
            break; 
        }
    }
    fclose(file);
    *lines = line_count;
    return 0;
}

int COUNT_LINES_IN_FILE_POSIX (char filename[], size_t *lines) {

    // Same function as before but posix

    size_t line_count = 0; // Counter starting at 0
    FILE *file;
    file = fopen(filename,"rb"); // Open file

    if (file == NULL) { // Check if you can open file
        fprintf(stderr, "Cannot open file.\n");
        return 1;
    }

    char buffer[BUF_SZ_4]; // Creates buffer with size of BUF_SZ_4
    while (1) {
        size_t bytes_read = fread(buffer, 1, BUF_SZ_4, file); // puts chars from file in to buffer and returns the amount of bytes.
        for (size_t i = 0 ; i < bytes_read; i++) { 
            if (buffer[i] == '\n') { // Searches through the buffer for newlines.
                line_count++;
            } 
        }
        if (feof(file)) { // If end of file is encountered then break
            break; 
        }
    }
    fclose(file);
    *lines = line_count;
    return 0;
}

void shuffle(char arr[], int n) {
    for (int i = n -1 ; i > 0 ; i--) { 
        int j = rand() % (i + 1); 
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

int GET_LINE(char filename[], long focus, char **line) { // Making this function was hell. Hardest thing ive coded in a while.
    
    size_t lines;
    int ret = COUNT_LINES_IN_FILE(filename, &lines);
    if (ret == 1) {
        return EXIT_FAILURE;
    }

    if ((long)lines < focus) {
        return EXIT_FAILURE;
    }

    FILE *file;
    file = fopen(filename,"r"); // Open file

    if (file == NULL) { // Check if you can open file
        fprintf(stderr, "Cannot open file.\n");
        return 1;
    }
    
    if (focus == 1) {
        int c1_count = 0;
        while (1) {
            char c = fgetc(file);
            if (c == '\n') {
                c1_count++;
                break;
            } else if (c == EOF) {
                break;
            } else {
                c1_count++;
            }
        }
        char c1buf[c1_count];
        fseek(file, 0, SEEK_SET);
        
        int i = 0;

        for (; i < c1_count ; i++) {
            c1buf[i] = fgetc(file);
        }
        c1buf[i] = '\0';
        *line = (char *)malloc(strlen(c1buf) + 1);

        if (*line != NULL) {
            strcpy(*line, c1buf);
        }
        //printf("%s", c1buf); // The purpose of this if statement is that it will only print line 1. Not too elegant of a way to handle this but its the only way i knew how to.
    } else {

        focus--;
        size_t line_count = 0; // Counter starting at 0
        size_t save_i;
        for (size_t i = 0; ; i++) {
            char c = fgetc(file);
            if (feof(file)) { // If end of file is encountered then break
                break; 
            }
            if (c == '\n') {
                line_count++;
                if (line_count == (size_t)focus) {
                    save_i = i;
                    break;
                }
            }
        }
        fseek(file, save_i+1, SEEK_SET);
        
        int c2_count = 0;
        while (1) {
            char c = fgetc(file);
            if (c == '\n') {
                c2_count++;
                break;
            } else if (c == EOF) {
                break;
            } else {
                c2_count++;
            }
        }
        
        fseek(file, save_i+1, SEEK_SET);
        char c2buf[c2_count];
        int i = 0;
        for (; i < c2_count ; i++) {
            c2buf[i] = fgetc(file);
        }
        c2buf[i] = '\0';
        *line = (char *)malloc(strlen(c2buf) + 1);

        if (*line != NULL) {
            strcpy(*line, c2buf);
        }
    }
    
fclose(file);
return 0;
}