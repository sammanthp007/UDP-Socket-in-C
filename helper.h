#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <errno.h>
#include <ctype.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <unistd.h>     // Misc UNIX function

#define MAX_LINE (1024) // max inside any buffer

void print_error_and_exit(char *msg)
{
    perror(msg);
    printf("\n");
    exit(0);
}

void Cap(char []);
void Cap(char string[]){
    int i;
    int x = strlen(string); // get the length of the whole string.
    for (i=0;i<x;i++){
        if (isalpha(string[i])){
            string[i]= toupper(string[i]);
        }
    }
}
