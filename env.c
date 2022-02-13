/*
    Elijah Ager Luckett
    CS 3393 - Unix
    Env file
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern const char** environ;

void printenv(char** env);
int env

int main(int argc, char* argv[]) {
    //variables
    char **myenv;
    int iflag=0;
    int index=0;
    if (argc == 1){
        printenv();
        return 0;
    }
    else if (!strcmp(argv[1], "-i")){
        iflag = 1;
        for (int i = 2; i < argc; i++) {
            if (strchr(argv[i], '=') == NULL) {
                index=i;
            }
        }
        if (index == 0){
            index=argc;
        }
        myenv=(char**) malloc((index - 2) * sizeof(char*));
    }
    else{// no -i
        iflag=0
        for (int i = 1; i < argc; i++) {
            if (strchr(argv[i], '=') == NULL) {
                index=i;
            }
        }
        if (index == 0){
            index=argc;
        }
        (char**) malloc((command_index - 1) * sizeof(char*));
        
    }
    if (command_index != argc){
        if(execvp(argv[command_index], &argv[command_index]) == -1){
            perror("Failed exec(): ");
            exit(EXIT_FAILURE);
        }
        return 0;
    }
    printenv();
    freeenv();
    return 0;
}

void printenv(char** env){
    for (int i = 0; env[i] != NULL; i++) {
        printf("%s\n", env[i]);
    }
}
