/*
    Elijah Ager Luckett
    CS 3393 - Unix
    Env file
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

extern char** environ;

void printenv(char** env);
int arrlen(char **arr);
int getindex(char **arr, char *val);
char **parseenv(int start, int argc, char *argv[]);
//char **alloci(int numvar, char* argv[]);
char **allocnoi(char* argv[], char *newenv[]);
void freeenv();
void runcommand(int start, char* argv[]);
int haveTheSameKey(char* pair1, char* pair2);

int main(int argc, char* argv[]) {
    //basic print env case
    if (argc == 1){
        printenv(environ);
        return 0;
    }
    //variables for modified env case
    bool modify = true;
    //char **newenv;
    int index=1; //as default I will just assume there is no -i
    if (!strcmp(argv[1], "-i")){
        modify = false;
        index = 2;
        if (argc == 2){
            exit(0);
        }
        char** newenv = parseenv(index,argc,argv);
        while(argv[index] && strchr(argv[index], '=')){
            index++;
        }
        //printf("here");
        environ = newenv;
    }
    else{// no -i
        modify = true;
        char** newenv = parseenv(index,argc,argv);
        while(argv[index] && strchr(argv[index], '=')){
            index++;
        }
        environ = allocnoi(argv,newenv);
    }
    //printf("here");
    if (index<argc){
        runcommand(index,argv);
    }
    else{
        printenv(environ);
        return 0;
    }
    printf("here");
    int length = arrlen(environ);// could be made into a function
    freeenv(length);
    return 0;
}

void printenv(char** env){
    for (int i = 0; env[i] != NULL; i++) {
        printf("%s\n", env[i]);
    }
}
int arrlen(char **arr){
    int count = 0;
    while(arr[count]){
        count++;
    }
    return count;
}
int haveTheSameKey(char* pair1, char* pair2) {
    while (*pair1 != '=') {
        if (*pair1 != *pair2) {
            return 0;
        }
        pair1++;
        pair2++;
    }
    return 1;
}
int getindex(char **arr, char *val) {
    int index = 0;
    int length = arrlen(arr);
    while(arr[index]){
        if (arr[index] == val) {
            return length;
        }
        if (!strcmp(arr[index], "")) {
            return index;
        }
        if (haveTheSameKey(val, arr[index])) {
            return index;
        }
        index++;
    }
    return index;
    
}
char **parseenv(int start, int argc, char *argv[]){
    int newlength = 0;
    int i = start;
    while (i<argc && strchr(argv[i], '=')){
        int lenargv=arrlen(argv);
        int index=getindex(argv,argv[i]);
        if(lenargv==index){
            newlength++;
        }
        i++;
    }
    //***NEED TO CHECK THESE MALLOCS
    char **newenv=(char **) malloc((newlength+1) * sizeof(char*));
    for (int i = 0; i < newlength; i++) {
        newenv[i] = (char *) malloc(sizeof(char*));
    }
    printf("%d\n",newlength);
    newenv[newlength]=NULL;
    for (int i=start; i < argc; i++) {
        if (strchr(argv[i], '=') != NULL) {
            printf("%s\n",argv[i]);
            int index=getindex(newenv,argv[i]);
            printf("%d",index);
            newenv[index]=argv[i];
        }
    }
    return newenv;
}

char **allocnoi(char* argv[], char* newenv[]){
    //int oldnumvar=arrlen(environ);
    int newnumvar = 0;
    for (char** ptr = newenv; *ptr != NULL; ptr++) {
        int lenOldEnv=arrlen(environ);
        int index=getindex(environ,*ptr);
        if(lenOldEnv==index){
            newnumvar++;
        }
    }
    char **merged=(char **) malloc((newnumvar + 1) * sizeof(char*));
    merged[newnumvar] = NULL;
    for (int i = 0; i < newnumvar; i++) {
        merged[i] = (char *) malloc(sizeof(char*));
    }
    int ptr = 0;
    for (int i = 0; environ[i] != NULL; i++, ptr++) {
        merged[ptr] = environ[i];
    }
    for (int i = 0; newenv[i] != NULL; i++) {
        int index = getindex(merged, newenv[i]);
        merged[index]=newenv[i];
    }
    return merged;
}

void freeenv(int envsize){
    for(int i = 0; i < envsize; i++){
        free(environ[i]);
    }
    free(environ);
}

void runcommand(int start, char* argv[]){
    int res=execvp(argv[start], argv+start);
    if(res == -1){
        int length = arrlen(environ);
        freeenv(length);
        perror("execvp failed\n");
    }
}
