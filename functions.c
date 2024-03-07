#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include "7ed.h"
#include <time.h>
#include <string.h>
#include <stdint.h>

int choice() {
    char choice;

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

int count_lines_in_file (char filename[], uint64_t *lines) {

    // Does not follow posix because this function accounts for if the last line does not end with a newline.

    uint64_t line_count = 0; // Counter starting at 0
    FILE *file;
    file = fopen(filename,"rb"); // Open file

    if (file == NULL) { // Check if you can open file
        fprintf(stderr, "count_lines_in_file(): Cannot open file.\n");
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

int count_lines_in_file_posix (char filename[], size_t *lines) {

    // Same function as before but posix

    size_t line_count = 0; // Counter starting at 0
    FILE *file;
    file = fopen(filename,"rb"); // Open file

    if (file == NULL) { // Check if you can open file
        fprintf(stderr, "Cannot open file.\n");
        return 1;
    }

    char buffer[BUF_SZ_4]; // Creates buffer with size of BUF_SZ_4
while (1) { // hello!
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

int get_line(char filename[], long focus, char **line, size_t *start) { // Making this function was hell. Hardest thing ive coded in a while.
    
    FILE *file;
    file = fopen(filename,"r"); // Open file

    if (file == NULL) { // Check if you can open file
        fprintf(stderr, "Cannot open file get_line.\n");
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
        }                       // checks how many characters are in the first line
        char c1buf[c1_count+1];
        fseek(file, 0, SEEK_SET);
        
        int i = 0;

        for (; i < c1_count ; i++) {
            c1buf[i] = fgetc(file);
        }
        c1buf[i] = '\0';
        *line = (char *)malloc(strlen(c1buf) + 1);

        if (*line != NULL) {
            strcpy(*line, c1buf); // Return line 1
        }

        *start = 0; // Is start the start of where line

    } else {

        focus--;
        size_t line_count = 0; // Counter starting at 0
        size_t save_i = 0;
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
        char c2buf[c2_count+1];
        int i = 0;
        for (; i < c2_count ; i++) {
            c2buf[i] = fgetc(file);
        }
        c2buf[i] = '\0';
        *line = (char *)malloc(strlen(c2buf) + 1);

        if (*line != NULL) {
            strcpy(*line, c2buf);
        }
        *start = save_i+1; // not sure but i think it saves the start position of the line
        
    }
    
    fclose(file);
    return 0;
}