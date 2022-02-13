/*
    Elijah Ager Luckett
    CS 3393 - Unix
    Env file
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printenv(char** environ);

int main(int argc, char* argv[]) {
    //variables
    char **myenv;
    int iflag;
    if (argc == 1){
        printenv();
        exit(0);
    }
    
}

void printenv(char** environ){
    for (int i = 0; environ[i] != NULL; i++) {
        printf("%s\n", environ[i]);
    }
}
