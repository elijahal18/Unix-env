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
    }
    
}

void printenv(char** env){
    for (int i = 0; env[i] != NULL; i++) {
        printf("%s\n", env[i]);
    }
}
