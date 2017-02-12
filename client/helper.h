#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <errno.h>
#include <ctype.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <unistd.h>     // Misc UNIX function

#define LISTENQ        (1024)   /*  Backlog for listen()   */
#define MAX_LINE (1024) // max inside any buffer

void print_error_and_exit(char *msg)
{
    perror(msg);
    printf("\n");
    exit(0);
}

void Cap(char []);
char* ReadFile(char *);

void Cap(char string[]){
    int i;
    int x = strlen(string); // get the length of the whole string.
    for (i=0;i<x;i++){
        if (isalpha(string[i])){
            string[i]= toupper(string[i]);
        }
    }
}


char* ReadFile(char *filename){
    char *buffer = NULL;
    int string_size, read_size;
    FILE *handler = fopen(filename, "r");

    if (handler)
    {
        // Seek the last byte of the file
        fseek(handler, 0, SEEK_END);
        // Offset from the first to the last byte, or in other words, filesize
        string_size = ftell(handler);
        // go back to the start of the file
        rewind(handler);

        // Allocate a string that can hold it all
        buffer = (char*) malloc(sizeof(char) * (string_size + 1) );

        // Read it all in one operation
        read_size = fread(buffer, sizeof(char), string_size, handler);

        // fread doesn't set it so put a \0 in the last position
        // and buffer is now officially a string
        buffer[string_size] = '\0';

        if (string_size != read_size)
        {
            // Something went wrong, throw away the memory and set
            // the buffer to NULL
            free(buffer);
            buffer = NULL;
        }

        // Always remember to close the file.
        fclose(handler);
    }

    return buffer;
}
