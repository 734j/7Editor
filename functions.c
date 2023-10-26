#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include "7ed.h"
#include <time.h>

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

int print_7ed() {

    printf("print_7ed()\n");

    return 0;

}